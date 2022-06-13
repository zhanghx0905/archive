import time
from copy import deepcopy

import numpy as np
import torch
from pytorch_pretrained_bert.optimization import BertAdam
from scipy.stats import pearsonr
from sklearn.metrics import accuracy_score, classification_report, f1_score
from torch.nn import CrossEntropyLoss
from torch.optim import Adam
from torch.utils.tensorboard import SummaryWriter

from dataset import class_list


class Stat:
    def __init__(self, training, writer=None):
        self.step = 0
        self.loss = []
        self.labels = []
        self.outputs = []
        self.pred_labels = []
        self.pred_outputs = []
        self.training = training
        self.writer = writer

    def add(self, pred_outs, gold_outs, loss):
        outs = gold_outs.cpu().numpy()
        pred_outs = pred_outs.cpu().detach().numpy()
        labels = np.argmax(outs, axis=1)
        pred_labels = np.argmax(pred_outs, axis=1)

        self.loss.append(loss)
        self.labels.extend(labels)
        self.outputs.extend(outs)
        self.pred_labels.extend(pred_labels)
        self.pred_outputs.extend(pred_outs)

    def eval(self):
        acc = accuracy_score(self.labels, self.pred_labels)
        f1 = f1_score(self.labels, self.pred_labels, average='macro')
        norm_gold = np.asarray(self.outputs)
        norm_pred = np.asarray(self.pred_outputs)
        corr = sum([pearsonr(norm_gold[i], norm_pred[i])[0]
                    for i in range(len(norm_gold))]) / len(norm_gold)
        return acc, f1, corr

    def log(self):
        self.step += 1
        acc, f1, coef = self.eval()
        loss = sum(self.loss) / len(self.loss)
        self.loss = []
        self.labels = []
        self.outputs = []
        self.pred_labels = []
        self.pred_outputs = []
        if not self.writer:
            return loss, acc, f1, coef
        if self.training:
            self.writer.add_scalar('train_Loss', loss, self.step)
            self.writer.add_scalar('train_Accuracy', acc, self.step)
        else:
            self.writer.add_scalar('dev_Loss', loss, self.step)
            self.writer.add_scalar('dev_Accuracy', acc, self.step)
        return loss, acc, f1, coef


def train(args, model, train_data_loader, dev_data_loader):
    loss_func = CrossEntropyLoss()

    if args['type'] == "BERT":
        param_optimizer = list(model.named_parameters())
        no_decay = ['bias', 'LayerNorm.bias', 'LayerNorm.weight']
        optimizer_grouped_parameters = [
            {'params': [p for n, p in param_optimizer if not any(
                nd in n for nd in no_decay)], 'weight_decay': 0.01},
            {'params': [p for n, p in param_optimizer if any(nd in n for nd in no_decay)], 'weight_decay': 0.0}]
        optimizer = BertAdam(optimizer_grouped_parameters,
                             lr=args['lr'],
                             warmup=0.05,
                             t_total=len(train_data_loader) *
                             args['num_epochs'],
                             weight_decay=args['weight_decay'])
    else:
        optimizer = Adam(model.parameters(),
                         args['lr'], weight_decay=args['weight_decay'])

    writer = SummaryWriter(args['output_path']+'/' +
                           time.strftime('%m-%d_%H.%M', time.localtime()))
    train_stat = Stat(training=True, writer=writer)
    dev_stat = Stat(training=False, writer=writer)

    best_acc, best_net = 0, None
    best_epoch = -1

    for epoch in range(args['num_epochs']):
        print(f"--- epoch: {epoch + 1} ---")
        print("--- training ---")
        for iter, batch in enumerate(train_data_loader):
            model.train()
            inputs, votes, labels = batch
            optimizer.zero_grad()
            pred_outputs = model(inputs)

            loss = loss_func(pred_outputs, labels)
            loss.backward()
            optimizer.step()
            train_stat.add(pred_outputs, votes, loss.item())

            if (iter + 1) % args['display_per_batch'] == 0:
                loss, acc, f1, coef = train_stat.log()
                print(f"step {(iter + 1):2}, "
                      f"loss: {loss:.4f}, acc: {acc:.2%}, "
                      f"f1: {f1:.4f}, coef: {coef:.4f}")
        # validate per epoch
        print("--- validating ---")
        model.eval()
        with torch.no_grad():
            for batch in dev_data_loader:
                inputs, votes, labels = batch
                pred_outputs = model(inputs)

                loss = loss_func(pred_outputs, labels)

                dev_stat.add(pred_outputs, votes, loss.item())
        loss, acc, f1, coef = dev_stat.log()
        print(f"loss: {loss:.4f}, acc: {acc:.2%}, "
              f"f1: {f1:.4f}, coef: {coef:.4f}")

        if acc > best_acc:
            best_acc, best_epoch = acc, epoch
            best_net = deepcopy(model.state_dict())

    print(f"best dev acc: {best_acc:.4f}, epoch: {best_epoch}")
    return best_net


def test(model, test_data_loader):
    test_stat = Stat(training=False)
    print("--- testing ---")
    model.eval()
    with torch.no_grad():
        for batch in test_data_loader:
            inputs, votes, _ = batch
            pred_outputs = model(inputs)
            test_stat.add(pred_outputs, votes, 0)  # dummy for loss
    acc, f1, corr = test_stat.eval()
    print(f"corr: {corr:.4f}")
    report = classification_report(
        test_stat.labels,
        test_stat.pred_labels,
        target_names=class_list,
        digits=4, zero_division=0)
    print(report)
