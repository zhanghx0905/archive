'''
Modifed by zhanghx on 8.6
'''
import os
import re
import numpy as np
import networkx as nx
from collections import Counter
from copy import deepcopy

import numpy

from utils import compute_ppr, normalize_adj, adj_corrupt


def download(dataset):
    basedir = os.path.dirname(os.path.abspath(__file__))
    datadir = os.path.join(basedir, 'data', dataset)
    if not os.path.exists(datadir):
        os.makedirs(datadir)
        url = 'https://ls11-www.cs.tu-dortmund.de/people/morris/graphkerneldatasets/{0}.zip'.format(
            dataset)
        zipfile = os.path.basename(url)
        os.system('wget {0}; unzip {1}'.format(url, zipfile))
        os.system('mv {0}/* {1}'.format(dataset, datadir))
        os.system('rm -r {0}'.format(dataset))
        os.system('rm {0}'.format(zipfile))


def process(dataset):
    src = os.path.join(os.path.dirname(__file__), 'data')
    prefix = os.path.join(src, dataset, dataset)

    graph_node_dict = {}
    with open('{0}_graph_indicator.txt'.format(prefix), 'r') as f:
        for idx, line in enumerate(f):
            graph_node_dict[idx + 1] = int(line.strip('\n'))
    max_nodes = Counter(graph_node_dict.values()).most_common(1)[0][1]

    node_labels = []
    if os.path.exists('{0}_node_labels.txt'.format(prefix)):
        with open('{0}_node_labels.txt'.format(prefix), 'r') as f:
            for line in f:
                node_labels += [int(line.strip('\n')) - 1]
            num_unique_node_labels = max(node_labels) + 1
    else:
        print('No node labels')

    node_attrs = []
    if os.path.exists('{0}_node_attributes.txt'.format(prefix)):
        with open('{0}_node_attributes.txt'.format(prefix), 'r') as f:
            for line in f:
                node_attrs.append(
                    np.array([float(attr) for attr in re.split(
                        "[,\s]+", line.strip("\s\n")) if attr], dtype=np.float)
                )
    else:
        print('No node attributes')

    graph_labels = []
    unique_labels = set()
    with open('{0}_graph_labels.txt'.format(prefix), 'r') as f:
        for line in f:
            val = int(line.strip('\n'))
            if val not in unique_labels:
                unique_labels.add(val)
            graph_labels.append(val)
    label_idx_dict = {val: idx for idx, val in enumerate(unique_labels)}
    graph_labels = np.array([label_idx_dict[l] for l in graph_labels])

    adj_list = {idx: [] for idx in range(1, len(graph_labels) + 1)}
    index_graph = {idx: [] for idx in range(1, len(graph_labels) + 1)}
    with open('{0}_A.txt'.format(prefix), 'r') as f:
        for line in f:
            u, v = tuple(map(int, line.strip('\n').split(',')))
            adj_list[graph_node_dict[u]].append((u, v))
            index_graph[graph_node_dict[u]] += [u, v]

    for k in index_graph.keys():
        index_graph[k] = [u - 1 for u in set(index_graph[k])]

    graphs, pprs = [], []
    for idx in range(1, 1 + len(adj_list)):
        graph = nx.from_edgelist(adj_list[idx])
        if max_nodes is not None and graph.number_of_nodes() > max_nodes:
            continue

        graph.graph['label'] = graph_labels[idx - 1]
        for u in graph.nodes():
            if len(node_labels) > 0:
                node_label_one_hot = [0] * num_unique_node_labels
                node_label = node_labels[u - 1]
                node_label_one_hot[node_label] = 1
                graph.nodes[u]['label'] = node_label_one_hot
            if len(node_attrs) > 0:
                graph.nodes[u]['feat'] = node_attrs[u - 1]
        if len(node_attrs) > 0:
            graph.graph['feat_dim'] = node_attrs[0].shape[0]

        # relabeling
        mapping = {}
        for node_idx, node in enumerate(graph.nodes()):
            mapping[node] = node_idx

        graphs.append(nx.relabel_nodes(graph, mapping))
        pprs.append(compute_ppr(graph, alpha=0.2))

    if 'feat_dim' in graphs[0].graph:
        pass
    else:
        max_deg = max([max(dict(graph.degree).values()) for graph in graphs])
        for graph in graphs:
            for u in graph.nodes(data=True):
                f = np.zeros(max_deg + 1)
                f[graph.degree[u[0]]] = 1.0
                if 'label' in u[1]:
                    f = np.concatenate(
                        (np.array(u[1]['label'], dtype=np.float), f))
                graph.nodes[u[0]]['feat'] = f
    return graphs, pprs


def _load(dataset):
    basedir = os.path.dirname(os.path.abspath(__file__))
    datadir = os.path.join(basedir, 'data', dataset)
    try:
        adj = np.load(f'{datadir}/adj.npy', allow_pickle=True)
        diff = np.load(f'{datadir}/diff.npy', allow_pickle=True)
        feat = np.load(f'{datadir}/feat.npy', allow_pickle=True)
        labels = np.load(f'{datadir}/labels.npy', allow_pickle=True)
    except FileNotFoundError:
        download(dataset)
        graphs, diff = process(dataset)
        feat, adj, labels = [], [], []
        from tqdm import tqdm
        pbar = tqdm(graphs)
        for graph in pbar:
            adj.append(nx.to_numpy_array(graph, dtype=np.float32))
            labels.append(graph.graph['label'])
            feat.append(np.array(list(nx.get_node_attributes(
                graph, 'feat').values()), dtype=np.float32))

        adj, diff, feat, labels = np.array(adj), np.array(
            diff), np.array(feat), np.array(labels)

        np.save(f'{datadir}/adj.npy', adj)
        np.save(f'{datadir}/diff.npy', diff)
        np.save(f'{datadir}/feat.npy', feat)
        np.save(f'{datadir}/labels.npy', labels)

    return adj, diff, feat, labels


def load(args, dataset: str):
    adj, diff, feat, labels = _load(dataset)
    num_nodes = np.array([a.shape[0] for a in adj])
    max_nodes = np.max(num_nodes)

    feat_dim = feat[0].shape[-1]
    feat_diff = deepcopy(feat)

    for idx in range(adj.shape[0]):
        adj[idx] = normalize_adj(adj[idx]).todense()

        if args.adj_corrupt:
            diff[idx] = adj_corrupt(adj[idx], args.corrupt_prob)

        if args.feat_permutate:
            feat_diff[idx] = np.random.permutation(feat_diff[idx])

        feat_diff[idx] = np.vstack((feat_diff[idx], np.zeros((max_nodes - feat[idx].shape[0], feat_dim))))
        feat[idx] = np.vstack((feat[idx], np.zeros((max_nodes - feat[idx].shape[0], feat_dim))))

        diff[idx] = np.hstack(
            (np.vstack((diff[idx], np.zeros((max_nodes - diff[idx].shape[0], diff[idx].shape[0])))),
             np.zeros((max_nodes, max_nodes - diff[idx].shape[1]))))
        adj[idx] = np.hstack(
            (np.vstack((adj[idx], np.zeros((max_nodes - adj[idx].shape[0], adj[idx].shape[0])))),
             np.zeros((max_nodes, max_nodes - adj[idx].shape[1]))))

    adj = np.array(adj.tolist()).reshape(-1, max_nodes, max_nodes)
    diff = np.array(diff.tolist()).reshape(-1, max_nodes, max_nodes)
    feat = np.array(feat.tolist()).reshape(-1, max_nodes, feat_dim)
    feat_diff = np.array(feat_diff.tolist()).reshape(-1, max_nodes, feat_dim)

    if args.adjadj or args.feat_permutate or args.rwr or args.dropnode:
        diff = adj
    elif args.pprppr:
        adj = diff
    return adj, diff, feat, feat_diff, labels, num_nodes


if __name__ == '__main__':
    # MUTAG, PTC_MR, IMDB-BINARY, IMDB-MULTI, REDDIT-BINARY, REDDIT-MULTI-5K,
    adj, diff, feat, labels = _load('PTC_MR')
    print('done')
