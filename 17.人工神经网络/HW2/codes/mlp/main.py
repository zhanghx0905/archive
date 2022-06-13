# -*- coding: utf-8 -*-
import argparse
import os
import time

import numpy as np
import torch
import torch.optim as optim
from torch.utils.tensorboard.writer import SummaryWriter

from load_data import load_cifar_2d
from model import Model
from utils import Stat, Result

os.environ['CUDA_VISIBLE_DEVICES'] = '2'
parser = argparse.ArgumentParser()

parser.add_argument('--batch_size', type=int, default=100,
                    help='Batch size for mini-batch training and evaluating. Default: 100')
parser.add_argument('--num_epochs', type=int, default=60,
                    help='Number of training epoch.')
parser.add_argument('--learning_rate', type=float, default=1e-3,
                    help='Learning rate during optimization. Default: 1e-3')
parser.add_argument('--drop_rate', type=float, default=0.5,
                    help='Drop rate of the Dropout Layer. Default: 0.5')
parser.add_argument('--is_train', type=bool, default=True,
                    help='True to train and False to inference. Default: True')
parser.add_argument('--data_dir', type=str, default='../cifar-10_data',
                    help='Data directory. Default: ../cifar-10_data')
parser.add_argument('--nobm', action='store_true')
parser.add_argument('--verbose', action='store_true')
args = parser.parse_args()


def shuffle(X, y, shuffle_parts):
    chunk_size = int(len(X) / shuffle_parts)
    shuffled_range = list(range(chunk_size))

    X_buffer = np.copy(X[0:chunk_size])
    y_buffer = np.copy(y[0:chunk_size])

    for k in range(shuffle_parts):
        np.random.shuffle(shuffled_range)
        for i in range(chunk_size):
            X_buffer[i] = X[k * chunk_size + shuffled_range[i]]
            y_buffer[i] = y[k * chunk_size + shuffled_range[i]]

        X[k * chunk_size:(k + 1) * chunk_size] = X_buffer
        y[k * chunk_size:(k + 1) * chunk_size] = y_buffer

    return X, y


def train_epoch(model, X, y, optimizer):  # Training Process
    model.train()
    loss, acc = 0.0, 0.0
    st, ed, times = 0, args.batch_size, 0
    while st < len(X) and ed <= len(X):
        optimizer.zero_grad()
        X_batch, y_batch = torch.FloatTensor(X[st:ed]).to(device), torch.LongTensor(y[st:ed]).to(device)
        loss_, acc_ = model(X_batch, y_batch)

        loss_.backward()
        optimizer.step()

        loss += loss_.cpu().data.numpy()
        acc += acc_.cpu().data.numpy()
        st, ed = ed, ed + args.batch_size
        times += 1
    loss /= times
    acc /= times
    return acc, loss


def valid_epoch(model, X, y):  # Valid Process
    model.eval()
    loss, acc = 0.0, 0.0
    st, ed, times = 0, args.batch_size, 0
    while st < len(X) and ed <= len(X):
        X_batch, y_batch = torch.FloatTensor(X[st:ed]).to(device), torch.LongTensor(y[st:ed]).to(device)
        loss_, acc_ = model(X_batch, y_batch)

        loss += loss_.cpu().data.numpy()
        acc += acc_.cpu().data.numpy()

        st, ed = ed, ed + args.batch_size
        times += 1
    loss /= times
    acc /= times
    return acc, loss


def inference(model, X):  # Test Process
    model.eval()
    pred_ = model(torch.from_numpy(X).to(device))
    return pred_.cpu().data.numpy()


if __name__ == '__main__':
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    X_train, X_test, y_train, y_test = load_cifar_2d(args.data_dir)
    X_val, y_val = X_train[40000:], y_train[40000:]
    X_train, y_train = X_train[:40000], y_train[:40000]

    mlp_model = Model(drop_rate=args.drop_rate, bm=not args.nobm)
    mlp_model.to(device)
    if args.verbose:
        print(mlp_model)
    optimizer = optim.Adam(mlp_model.parameters(), lr=args.learning_rate)

    pre_losses = [1e18] * 3
    best_val_acc = 0.0

    writer_path = f'../log/mlp_drop{args.drop_rate}'
    if not args.nobm:
        writer_path += '_bm'

    os.makedirs(writer_path, exist_ok=True)
    writer = SummaryWriter(writer_path)
    train_stat = Stat('train', writer=writer)
    val_stat = Stat('val', writer=writer)
    result = None

    for epoch in range(1, args.num_epochs + 1):
        start_time = time.time()
        train_acc, train_loss = train_epoch(mlp_model, X_train, y_train, optimizer)
        X_train, y_train = shuffle(X_train, y_train, 1)

        val_acc, val_loss = valid_epoch(mlp_model, X_val, y_val)

        train_stat.log(train_loss, train_acc)
        val_stat.log(val_loss, val_acc)

        if val_acc >= best_val_acc:
            best_val_acc = val_acc
            best_epoch = epoch
            test_acc, test_loss = valid_epoch(mlp_model, X_test, y_test)
            result = Result(val_acc, val_loss, train_acc, train_loss)
        
        if args.verbose:
            epoch_time = time.time() - start_time
            print(f"Epoch {epoch} of {args.num_epochs} took {str(epoch_time)}s")
            print("  learning rate:                 " + str(optimizer.param_groups[0]['lr']))
            print("  training loss:                 " + str(train_loss))
            print("  training accuracy:             " + str(train_acc))
            print("  validation loss:               " + str(val_loss))
            print("  validation accuracy:           " + str(val_acc))
            print("  best epoch:                    " + str(best_epoch))
            print("  best validation accuracy:      " + str(best_val_acc))
            print("  test loss:                     " + str(test_loss))
            print("  test accuracy:                 " + str(test_acc))

        if train_loss > max(pre_losses):
            for param_group in optimizer.param_groups:
                param_group['lr'] = param_group['lr'] * 0.9995
        pre_losses = pre_losses[1:] + [train_loss]
    print(f"dropout = {args.drop_rate}, bm = {not args.nobm}")
    print(result)
    print(f"Test Acc:  {test_acc:.2%}\n"
          f"Test Loss: {test_loss:.5f}")
