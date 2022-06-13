import argparse
import os
from collections import namedtuple

import numpy as np
from torch.utils.tensorboard import SummaryWriter

from layers import Gelu, Linear, Relu, Sigmoid
from load_data import load_mnist_2d
from loss import EuclideanLoss, HingeLoss, SoftmaxCrossEntropyLoss
from network import Network
from solve_net import test_net, train_net
from utils import LOG_INFO


class EarlyStopping:
    def __init__(self, patience=10):
        self.patience = patience
        self.counter = 0
        self.best_score = None
        self.early_stop = False

    def step(self, score):
        if self.best_score is None:
            self.best_score = score
        elif score < self.best_score:
            self.counter += 1
            if self.counter >= self.patience:
                self.early_stop = True
                print('EarlyStopping!')
        else:
            self.best_score = score
            self.counter = 0
        return self.early_stop


class Stat:
    def __init__(self, training, writer: SummaryWriter):
        self.step = 0
        self.training = training
        self.writer = writer

    def log(self, loss, acc):
        self.step += 1
        if self.training:
            self.writer.add_scalar('Train_Loss', loss, self.step)
            self.writer.add_scalar('Train_Acc', acc, self.step)
        else:
            self.writer.add_scalar('Test_Loss', loss, self.step)
            self.writer.add_scalar('Test_Acc', acc, self.step)


train_data, test_data, train_label, test_label = load_mnist_2d('data')
INIT_STD = 0.01
ACTIVATIONS = {'relu': Relu, 'sigmoid': Sigmoid, 'gelu': Gelu}
LOSSES = {'l2': EuclideanLoss,
          'entropy': SoftmaxCrossEntropyLoss, 'hinge': HingeLoss}
Result = namedtuple('Result', 'Test_Acc Test_loss Train_Acc Traic_loss')


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--layers', type=int, default=2, help='1 or 2')
    parser.add_argument('--loss', type=str, default='entropy',
                        help='one of l2, entropy and hinge')
    parser.add_argument('--acti', type=str, default='relu',
                        help='one of sigmoid, relu and gelu')
    parser.add_argument('--verbose', action='store_true')
    return parser.parse_args()


args = parse_args()
acti = ACTIVATIONS[args.acti]
loss_fun = LOSSES[args.loss](name='loss')

# Your model defintion here
# You should explore different model architecture
model = Network()
if args.layers == 1:
    model.add(Linear('fc1', 784, 256, INIT_STD))
    model.add(acti('acti1'))
    model.add(Linear('fc2', 256, 10, INIT_STD))
else:
    model.add(Linear('fc1', 784, 512, INIT_STD))
    model.add(acti('acti1'))
    model.add(Linear('fc2', 512, 256, INIT_STD))
    model.add(acti('acti2'))
    model.add(Linear('fc3', 256, 10, INIT_STD))


# Training configuration
# You should adjust these hyperparameters
# NOTE: one iteration means model forward-backwards one batch of samples.
#       one epoch means model has gone through all the training samples.
#       'disp_freq' denotes number of iterations in one epoch to display information.

config = {
    'learning_rate': 0.01,
    'weight_decay': 0.0,
    'momentum': 0.9,
    'batch_size': 100,
    'max_epoch': 100,
    'disp_freq': 50,
    'test_epoch': 1
}

stopper = EarlyStopping()
os.makedirs('log', exist_ok=True)
writer = SummaryWriter(
    f'log/layers_{args.layers}_loss_{args.loss}_act_{args.acti}')
train_stat = Stat(training=True, writer=writer)
test_stat = Stat(training=False, writer=writer)

for epoch in range(config['max_epoch']):
    if args.verbose:
        LOG_INFO('Training @ %d epoch...' % (epoch))
    train_losses, train_accs = train_net(model, loss_fun, config, train_data, train_label,
                                         config['batch_size'], config['disp_freq'], args.verbose)
    for train_loss, train_acc in zip(train_losses, train_accs):
        train_stat.log(train_loss, train_acc)

    if epoch % config['test_epoch'] == 0:
        if args.verbose:
            LOG_INFO('Testing @ %d epoch...' % (epoch))
        test_loss, test_acc = test_net(model, loss_fun, test_data,
                                       test_label, config['batch_size'], args.verbose)
        res = Result(test_acc, test_loss, np.mean(train_accs), np.mean(train_losses))
        test_stat.log(test_loss, test_acc)
        if stopper.step(res):
            break

print(f'layers: {args.layers}, loss: {args.loss}, acti: {args.acti}\n'
      f'Test Acc = {stopper.best_score[0]:.2%}\n'
      f'Test Loss = {stopper.best_score[1]:.4f}\n'
      f'Train Acc  = {stopper.best_score[2]:.2%}\n'
      f'Train Loss = {stopper.best_score[3]:.4f}')
