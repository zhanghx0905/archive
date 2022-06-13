from functools import partial

import dgl
import numpy as np
import scipy.sparse as sp
import torch
from sklearn.model_selection import GridSearchCV, StratifiedKFold

from dataset import load
from model import Model, local_global_loss_
from utils import (EarlyStopping, ParserBase, drop_node, padding,
                   random_walk_with_restart)


class Parser(ParserBase):
    def __init__(self):
        super().__init__()
        self.parser.add_argument('config', type=str)

    def parse(self):
        args = super().parse()
        import json
        with open(args.config, 'r') as f:
            config = json.load(f)
        assert config['dataset'] in ['MUTAG', 'PTC_MR',
                                     'IMDB-BINARY', 'IMDB-MULTI', 'REDDIT-BINARY']
        return args, config


parser = Parser()
args, config = parser.parse()
seed = args.seed
torch.manual_seed(seed)
torch.backends.cudnn.deterministic = True
torch.backends.cudnn.benchmark = False
np.random.seed(seed)


def pretrain(
        adj, diff, feat, feat_diff, num_nodes,
        num_layer, epoch, batch):
    nb_epochs = epoch
    batch_size = batch
    patience = 20
    lr = 0.001 
    l2_coef = 0.0
    hid_units = 512
    restart_prob = 0.8
    step = 0
    prob = 0.5
    rwr_sampler = 1.1

    ft_size = feat[0].shape[1]
    max_nodes = feat[0].shape[0]

    model = Model(ft_size, hid_units, num_layer)
    optimiser = torch.optim.Adam(
        model.parameters(), lr=lr, weight_decay=l2_coef)

    model.cuda()
    stopper = EarlyStopping(patience)
    itr = (adj.shape[0] // batch_size) + 1

    from tqdm import trange
    for epoch in trange(nb_epochs):
        epoch_loss = 0.0
        train_idx = np.arange(adj.shape[0])
        np.random.shuffle(train_idx)

        for idx in range(0, len(train_idx), batch_size):
            model.train()
            optimiser.zero_grad()

            batch = train_idx[idx: idx + batch_size]
            mask = num_nodes[idx: idx + batch_size]
            # mask = num_nodes[batch]
            ba, bd = [], []
            bf, bdf = [], []
            nodes = max_nodes

            if args.rwr:
                # 提取batch中最小图的一部分
                mask = num_nodes[batch]
                nodes = int(min(mask)*rwr_sampler)
                g_padder = partial(padding, size=nodes)
                f_padder = partial(padding, size=(nodes, ft_size))
                for idx, nodes_num in zip(batch, mask):
                    g = dgl.DGLGraph(sp.coo_matrix(adj[idx], dtype=np.float32))
                    seeds = np.repeat(np.random.randint(0, nodes_num), 2)
                    subva, subvd = random_walk_with_restart(g, seeds, restart_prob, nodes)
                    area = np.ix_(subva, subva)
                    ba.append(g_padder(adj[idx][area]))
                    bd.append(g_padder(adj[idx][area]))
                    bf.append(f_padder(feat[idx][subva]))
                    bdf.append(f_padder(feat_diff[idx][subvd]))
            else:
                ba = adj[batch]
                bf = feat[batch]
                bd = diff[batch]
                if args.dropnode:
                    bdiff = feat_diff[batch]
                    for idx, adj_, feat_ in zip(batch, ba, bdiff): 
                        bdf.append(drop_node(adj_, feat_, prob, step, idx))
                else:
                    bdf = feat_diff[batch]

            ba = torch.FloatTensor(ba).cuda()
            bd = torch.FloatTensor(bd).cuda()
            bf = torch.FloatTensor(bf).cuda()
            bdf = torch.FloatTensor(bdf).cuda()

            lv1, gv1, lv2, gv2 = model(ba, bd, bf, bdf, mask)
            lv1 = lv1.view(batch.shape[0] * nodes, -1)
            lv2 = lv2.view(batch.shape[0] * nodes, -1)

            batch = torch.LongTensor(
                np.repeat(np.arange(batch.shape[0]), nodes)).cuda()

            loss1 = local_global_loss_(lv1, gv2, batch, 'JSD', mask)
            loss2 = local_global_loss_(lv2, gv1, batch, 'JSD', mask)
            # loss3 = global_global_loss_(gv1, gv2, 'JSD')
            loss = loss1 + loss2  # + loss3
            epoch_loss += loss
            loss.backward()
            optimiser.step()

        epoch_loss /= itr

        # print('Epoch: {0}, Loss: {1:0.4f}'.format(epoch, epoch_loss))
        if stopper.step(epoch_loss, model):
            break

    stopper.load_checkpoint(model)
    feat = torch.FloatTensor(feat).cuda()
    feat_diff = torch.FloatTensor(feat_diff).cuda()
    diff = torch.FloatTensor(diff).cuda()
    adj = torch.FloatTensor(adj).cuda()

    embeds = model.embed(feat, adj, diff, feat_diff, num_nodes)
    return embeds


def test(embeds, labels):
    labels = torch.LongTensor(labels).cuda()

    x = embeds.cpu().numpy()
    y = labels.cpu().numpy()

    from sklearn.svm import LinearSVC
    from sklearn.metrics import accuracy_score
    params = {'C': [0.001, 0.01, 0.1, 1, 10, 100, 1000]}
    kf = StratifiedKFold(n_splits=10, shuffle=True, random_state=None)
    accs = []
    for train_index, test_index in kf.split(x, y):
        x_train, x_test = x[train_index], x[test_index]
        y_train, y_test = y[train_index], y[test_index]
        classifier = GridSearchCV(LinearSVC(), params, cv=5, scoring='accuracy', verbose=0)
        classifier.fit(x_train, y_train)
        accs.append(accuracy_score(y_test, classifier.predict(x_test)))
    return np.mean(accs)


if __name__ == '__main__':
    import warnings
    warnings.filterwarnings("ignore")
    torch.cuda.set_device(args.gpu)
    
    adj, diff, feat, feat_diff, labels, num_nodes = load(args, config['dataset'])
    embeds = pretrain(adj, diff, feat, feat_diff, num_nodes,
                      num_layer=config['layers'], epoch=config['epochs'], batch=config['batchsize'])

    mean = test(embeds, labels)
    print(f"Dataset {config['dataset']}, Seed: {seed}, Acc mean {mean:.2%}")
