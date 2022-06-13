from collections import Iterable

import dgl
import networkx as nx
import numpy as np
import scipy.sparse as sp
import torch
from scipy.linalg import fractional_matrix_power, inv


class EarlyStopping:
    def __init__(self, patience=20):
        self.patience = patience
        self.counter = 0
        self.best_score = None
        self.best_state = None
        self.early_stop = False

    def step(self, loss, model):
        score = loss
        if self.best_score is None:
            self.best_score = score
            self.save_checkpoint(model)
        elif score > self.best_score:
            self.counter += 1
            if self.counter >= self.patience:
                self.early_stop = True
                # print('EarlyStopping!')
        else:
            self.best_score = score
            self.save_checkpoint(model)
            self.counter = 0
        return self.early_stop

    def save_checkpoint(self, model):
        '''Saves model when validation acc increase.'''
        from copy import deepcopy
        self.best_state = deepcopy(model.state_dict())

    def load_checkpoint(self, model):
        model.load_state_dict(self.best_state)
        self.best_state = None


class ParserBase:
    def __init__(self):
        import argparse
        self.parser = argparse.ArgumentParser()
        self.parser.add_argument('--pprppr', action='store_true',
                                 help='ppr-ppr contrast')
        self.parser.add_argument('--adjadj', action='store_true',
                                 help='adj-adj contrast')
        self.parser.add_argument('--feat-permutate',
                                 action='store_true', help='feature permutation')
        self.parser.add_argument('--adj-corrupt', action='store_true', help='adjacency matrix corruption')
        self.parser.add_argument('--dropnode', action='store_true', help='DropNode')
        self.parser.add_argument('--corrupt-prob', type=float, default=1e-4)
        self.parser.add_argument(
            '--rwr', action='store_true', help='random walk with restart')
        self.parser.add_argument('--gpu', type=int, default=None)
        self.parser.add_argument('--seed', type=int, default=123)

    def parse(self):
        args = self.parser.parse_args()
        return args


def accuracy(logits: torch.tensor, labels: torch.tensor) -> float:
    _, indices = torch.max(logits, dim=1)
    correct = torch.sum(indices == labels)
    return correct.item() * 1. / len(labels)


def compute_ppr(graph: nx.Graph, alpha=0.2, self_loop=True):
    a = nx.convert_matrix.to_numpy_array(graph)
    if self_loop:
        # A^ = A + I_n
        a = a + np.eye(a.shape[0])
    # D^ = Sigma A^_ii
    d = np.diag(np.sum(a, 1))
    dinv = fractional_matrix_power(d, -0.5)                       # D^(-1/2)
    # A~ = D^(-1/2) x A^ x D^(-1/2)
    at = np.matmul(np.matmul(dinv, a), dinv)
    # a(I_n-(1-a)A~)^-1
    return alpha * inv((np.eye(a.shape[0]) - (1 - alpha) * at))


def compute_heat(graph: nx.Graph, t=5, self_loop=True):
    a = nx.convert_matrix.to_numpy_array(graph)
    if self_loop:
        a = a + np.eye(a.shape[0])
    d = np.diag(np.sum(a, 1))
    return np.exp(t * (np.matmul(a, inv(d)) - 1))


def sparse_to_tuple(sparse_mx):
    """Convert sparse matrix to tuple representation."""

    def to_tuple(mx):
        if not sp.isspmatrix_coo(mx):
            mx = mx.tocoo()
        coords = np.vstack((mx.row, mx.col)).transpose()
        values = mx.data
        shape = mx.shape
        return coords, values, shape

    if isinstance(sparse_mx, list):
        for i in range(len(sparse_mx)):
            sparse_mx[i] = to_tuple(sparse_mx[i])
    else:
        sparse_mx = to_tuple(sparse_mx)

    return sparse_mx


def preprocess_features(features):
    """Row-normalize feature matrix and convert to tuple representation"""
    rowsum = np.array(features.sum(1))
    r_inv = np.power(rowsum, -1).flatten()
    r_inv[np.isinf(r_inv)] = 0.
    r_mat_inv = sp.diags(r_inv)
    features = r_mat_inv.dot(features)
    if isinstance(features, np.ndarray):
        return features
    else:
        return features.todense(), sparse_to_tuple(features)


def normalize_adj(adj, self_loop=True):
    """Symmetrically normalize adjacency matrix."""
    if self_loop:
        adj = adj + sp.eye(adj.shape[0])
    adj = sp.coo_matrix(adj)
    rowsum = np.array(adj.sum(1))
    d_inv_sqrt = np.power(rowsum, -0.5).flatten()
    d_inv_sqrt[np.isinf(d_inv_sqrt)] = 0.
    d_mat_inv_sqrt = sp.diags(d_inv_sqrt)
    return adj.dot(d_mat_inv_sqrt).transpose().dot(d_mat_inv_sqrt).tocoo()


def sparse_mx_to_torch_sparse_tensor(sparse_mx):
    """Convert a scipy sparse matrix to a torch sparse tensor."""
    sparse_mx = sparse_mx.tocoo().astype(np.float32)
    indices = torch.from_numpy(
        np.vstack((sparse_mx.row, sparse_mx.col)).astype(np.int64))
    values = torch.from_numpy(sparse_mx.data)
    shape = torch.Size(sparse_mx.shape)
    return torch.sparse.FloatTensor(indices, values, shape)


def adj_corrupt(adj: np.matrix, prob: float):
    sigma = np.random.binomial(1, prob, adj.shape)
    return adj * sigma


def feature_permutation(feat: np.matrix):
    return np.random.permutation(feat)


def random_walk_with_restart(
    g: dgl.DGLGraph, seeds: list, restart_prob: float, counts: int
):
    traces = dgl.contrib.sampling.random_walk_with_restart(
        g, seeds=seeds,
        restart_prob=restart_prob,
        max_nodes_per_seed=1e3,
        max_visit_counts=1, max_frequent_visited_nodes=counts-1)

    subvs = []
    for seed, trace in zip(seeds, traces):
        subv = torch.unique(torch.cat(trace)).tolist()[:counts-1]
        subv += [seed]
        '''
        if (counts != len(subv)):
            raise ValueError(f'len(subv): {len(subv)} != counts: {counts}')
        '''
        subvs.append(subv)
    return subvs


def padding(mat: np.matrix, size) -> np.matrix:
    ''' pad matrix with zeros '''
    if isinstance(size, int):
        return np.pad(mat, ((0, size-mat.shape[0]), (0, size-mat.shape[1])))
    elif isinstance(size, Iterable):
        pad_width = [(0, i-j) for i, j in zip(size, mat.shape)]
        return np.pad(mat, pad_width)
    else:
        raise NotImplementedError()


adj_powers_dict = {}


def drop_node(
    adj: np.matrix, feat: np.matrix, prob: float, step: int, idx = None
) -> np.matrix:
    ''' 
    Note that adj is the symmetric normalized adjacency matrix.
    idx is used to identify the index of input graph.
    '''
    if idx in adj_powers_dict:
        adj_powers = adj_powers_dict[idx]
        # print('hit')
    else:
        adj_powers = [np.linalg.matrix_power(adj, i) for i in range(0, step+1)]
        adj_powers_dict[idx] = adj_powers
    epsilon = np.random.binomial(1, 1-prob, feat.shape[0])[:, np.newaxis]
    perturbed = epsilon*feat/(1-prob)
    res = sum(adj_power@perturbed for adj_power in adj_powers)/(step+1)
    # print(feat.shape, res.shape)
    return res
