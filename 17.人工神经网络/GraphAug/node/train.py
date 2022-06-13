from functools import partial

import dgl
import numpy as np
import optuna
import scipy.sparse as sp
import torch
import torch.nn as nn

from dataset import load
from model import LogReg, Model
from utils import (EarlyStopping, ParserBase, accuracy, drop_node, padding,
                   random_walk_with_restart)


class Parser(ParserBase):
    def __init__(self):
        super().__init__()
        self.parser.add_argument('dataset', type=str)

    def parse(self):
        args = super().parse()
        assert args.dataset in ['cora', 'citeseer', 'pubmed']
        return args


parser = Parser()
args = parser.parse()
seed = args.seed
torch.manual_seed(seed)
torch.backends.cudnn.deterministic = True
torch.backends.cudnn.benchmark = False
dgl.random.seed(seed)
np.random.seed(seed)
device = torch.device(f'cuda:{args.gpu}'if args.gpu else 'cpu')


def pretrain(adj, diff, features, diff_feat, restart_prob=None, sample_size=None):
    nb_epochs = 2000
    patience = 20
    batch_size = 4

    lr = 0.001
    l2_coef = 0.0
    hid_units = 512
    sparse = False

    if sample_size is None:
        sample_size = 2000

    # for random walk with restart
    if restart_prob is None:
        restart_prob = 0.1

    # for dropnode
    steps = {'cora': 1, 'citeseer': 0, 'pubmed': 0}
    step = steps[args.dataset]

    ft_size = features.shape[1]
    if args.rwr:
        g = dgl.DGLGraph(sp.coo_matrix(adj, dtype=np.float32))
        if sample_size is None:
            sample_sizes = {'cora': 1000, 'citeseer': 900, 'pubmed': 1000}
            sample_size = sample_sizes[args.dataset]
        patience = 100  # bigger patience for rwr
        g_padder = partial(padding, size=sample_size)
        f_padder = partial(padding, size=(sample_size, ft_size))

    # Pretrain
    lbl_1 = torch.ones(batch_size, sample_size * 2)
    lbl_2 = torch.zeros(batch_size, sample_size * 2)
    lbl = torch.cat((lbl_1, lbl_2), 1)
    lbl = lbl.to(device)

    model = Model(ft_size, hid_units)
    model.to(device)
    optimiser = torch.optim.Adam(model.parameters(), lr=lr, weight_decay=l2_coef)

    b_xent = nn.BCEWithLogitsLoss()
    stopper = EarlyStopping(patience)

    from tqdm import trange
    for _ in trange(nb_epochs):
        ba, bd = [], []
        bf, bdf = [], []

        if args.rwr:
            # subsampling for two views using random walk with restart
            seeds = np.repeat(np.random.randint(0, adj.shape[-1], batch_size), 2)
            subvs = random_walk_with_restart(g, seeds, restart_prob, sample_size)
            subv_pairs = [subvs[i:i+2] for i in range(0, len(subvs), 2)]
            for subv1, subv2 in subv_pairs:
                ba.append(g_padder(adj[np.ix_(subv1, subv1)]))
                bd.append(g_padder(diff[np.ix_(subv2, subv2)]))
                bf.append(f_padder(features[subv1]))
                bdf.append(f_padder(diff_feat[subv2]))
        else:
            idx = np.random.randint(0, adj.shape[-1]-sample_size+1, batch_size)
            for i in idx:
                index = slice(i, i+sample_size)
                ba.append(adj[index, index])
                bd.append(diff[index, index])
                bf.append(features[index])
                if args.dropnode:
                    bdf.append(drop_node(ba[-1], features[index], 0.5, step, idx=i))
                else:
                    bdf.append(diff_feat[index])

        ba = np.array(ba).reshape(batch_size, sample_size, sample_size)
        bd = np.array(bd).reshape(batch_size, sample_size, sample_size)
        bf = np.array(bf).reshape(batch_size, sample_size, ft_size)
        bdf = np.array(bdf).reshape(batch_size, sample_size, ft_size)

        ba = torch.FloatTensor(ba).to(device)
        bd = torch.FloatTensor(bd).to(device)
        bf = torch.FloatTensor(bf).to(device)
        bdf = torch.FloatTensor(bdf).to(device)

        # permutate features for negative samples
        idx = np.random.permutation(sample_size)
        shuf_fts = bf[:, idx, :]
        shuf_fts = shuf_fts.to(device)

        model.train()
        optimiser.zero_grad()

        logits, __, __ = model(bf, bdf, shuf_fts, ba, bd,
                               sparse, None, None, None)

        loss = b_xent(logits, lbl)

        loss.backward()
        optimiser.step()

        # print('Epoch: {0}, Loss: {1:0.4f}'.format(epoch, loss.item()))
        if stopper.step(loss, model):
            stopper.load_checkpoint(model)
            break

    features = torch.FloatTensor(features[np.newaxis]).to(device)
    adj = torch.FloatTensor(adj[np.newaxis]).to(device)
    diff = torch.FloatTensor(diff[np.newaxis]).to(device)
    embeds, _ = model.embed(features, adj, diff, sparse, None)

    return embeds


def test(labels, idx_train, idx_test, embeds):
    ''' Downstream Tasks '''
    hid_units = 512
    nb_classes = np.unique(labels).shape[0]

    labels = torch.LongTensor(labels).to(device)
    idx_train = torch.LongTensor(idx_train).to(device)
    idx_test = torch.LongTensor(idx_test).to(device)

    train_embs = embeds[0, idx_train]
    test_embs = embeds[0, idx_test]

    train_lbls = labels[idx_train]
    test_lbls = labels[idx_test]

    accs = []
    wd = 0.01 if args.dataset == 'citeseer' else 0.0
    xent = nn.CrossEntropyLoss()

    for _ in range(50):
        log = LogReg(hid_units, nb_classes)
        opt = torch.optim.Adam(log.parameters(), lr=1e-2, weight_decay=wd)
        log.to(device)
        for _ in range(300):
            log.train()
            opt.zero_grad()

            logits = log(train_embs)
            loss = xent(logits, train_lbls)

            loss.backward()
            opt.step()

        logits = log(test_embs)

        accs.append(accuracy(logits, test_lbls))
    return np.mean(accs)


def hyper_optimize():
    adj, diff, features, diff_feat, labels, idx_train, idx_test = load(args)
    study = optuna.create_study(direction='maximize')

    def optimize(trial: optuna.trial.Trial):
        sample_size = trial.suggest_int('sample size', 400, 1000)
        restart_prob = trial.suggest_float('restart prob', 0.1, 0.25)
        embeds = pretrain(adj, diff, features, diff_feat, sample_size=sample_size, restart_prob=restart_prob)
        mean = test(labels, idx_train, idx_test, embeds)
        return mean
    
    study.optimize(optimize, n_trials=30, n_jobs=2)

    print("Best trial:")
    trial = study.best_trial
    print("  Value: ", trial.value)
    print("  Params: ")
    for key, value in trial.params.items():
        print("    {}: {}".format(key, value))


if __name__ == '__main__':
    import warnings
    warnings.filterwarnings("ignore")

    adj, diff, features, diff_feat, labels, idx_train, idx_test = load(args)

    embeds = pretrain(adj, diff, features, diff_feat)
    mean = test(labels, idx_train, idx_test, embeds)
    print(f'Dataset {args.dataset}, Acc mean {mean:.2%}')
