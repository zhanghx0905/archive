'''
Modifed by zhanghx on 8.6
'''
import os

import networkx as nx
import numpy as np
import scipy.sparse as sp
from dgl.data import CitationGraphDataset, CoraDataset
from sklearn.preprocessing import MinMaxScaler

from utils import adj_corrupt, compute_ppr, normalize_adj, preprocess_features


def download(dataset):
    if dataset == 'cora':
        return CoraDataset()
    elif dataset == 'citeseer' or 'pubmed':
        return CitationGraphDataset(name=dataset)
    else:
        return None


def _load(dataset):
    datadir = os.path.join('data', dataset)
    try:
        adj = np.load(f'{datadir}/adj.npy')
        diff = np.load(f'{datadir}/diff.npy')
        feat = np.load(f'{datadir}/feat.npy')
        labels = np.load(f'{datadir}/labels.npy')
        idx_train = np.load(f'{datadir}/idx_train.npy')
        idx_val = np.load(f'{datadir}/idx_val.npy')
        idx_test = np.load(f'{datadir}/idx_test.npy')
    except FileNotFoundError:
        os.makedirs(datadir)
        ds = download(dataset)
        adj = nx.to_numpy_array(ds.graph)
        diff = compute_ppr(ds.graph, 0.2)
        feat = ds.features[:]
        labels = ds.labels[:]

        idx_train = np.argwhere(ds.train_mask == 1).reshape(-1)
        idx_val = np.argwhere(ds.val_mask == 1).reshape(-1)
        idx_test = np.argwhere(ds.test_mask == 1).reshape(-1)

        np.save(f'{datadir}/adj.npy', adj)
        np.save(f'{datadir}/diff.npy', diff)
        np.save(f'{datadir}/feat.npy', feat)
        np.save(f'{datadir}/labels.npy', labels)
        np.save(f'{datadir}/idx_train.npy', idx_train)
        np.save(f'{datadir}/idx_val.npy', idx_val)
        np.save(f'{datadir}/idx_test.npy', idx_test)

    if dataset == 'citeseer':
        feat = preprocess_features(feat)

        epsilons = [1e-5, 1e-4, 1e-3, 1e-2]
        avg_degree = np.sum(adj) / adj.shape[0]
        epsilon = epsilons[np.argmin([abs(avg_degree - np.argwhere(diff >= e).shape[0] / diff.shape[0])
                                      for e in epsilons])]

        diff[diff < epsilon] = 0.0
        scaler = MinMaxScaler()
        scaler.fit(diff)
        diff = scaler.transform(diff)
    adj = normalize_adj(adj + sp.eye(adj.shape[0])).todense()
    return adj, diff, feat, labels, idx_train, idx_val, idx_test


def load(args):
    adj, diff, feat, labels, idx_train, _, idx_test = _load(args.dataset)

    diff_feat = feat
    if args.feat_permutate:
        diff_feat = np.random.permutation(feat)

    if args.adj_corrupt:  # set diff to adj coruption
        diff = adj_corrupt(adj, args.corrupt_prob)
    elif args.adjadj or args.rwr or args.feat_permutate or args.dropnode:
        diff = adj
    elif args.pprppr:
        adj = diff

    return adj, diff, feat, diff_feat, labels, idx_train, idx_test


if __name__ == '__main__':
    _load('cora')
