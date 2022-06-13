# -*- coding: utf-8 -*-

import torch
from torch import nn
from torch.nn import init
from torch.nn.parameter import Parameter


class BatchNorm2d(nn.Module):
    # TODO START
    def __init__(self, num_features):
        super(BatchNorm2d, self).__init__()
        self.num_features = num_features

        shape = (1, num_features, 1, 1)

        # Parameters
        self.weight = nn.Parameter(torch.ones(shape))
        self.bias = nn.Parameter(torch.zeros(shape))

        # Store the average mean and variance
        self.register_buffer('running_mean', torch.zeros(shape))
        self.register_buffer('running_var', torch.ones(shape))

        # Initialize your parameter
        self._momentum = 0.1
        self._eps = 1e-5

    def forward(self, input):
        # input: [batch_size, num_feature_map, height, width]
        if self.training:
            mean = input.mean((0, 2, 3), keepdims=True)
            var = input.var((0, 2, 3), unbiased=False, keepdims=True)
            self.running_mean = self._momentum * mean + (1 - self._momentum) * self.running_mean
            self.running_var = self._momentum * var + (1 - self._momentum) * self.running_var

            x_hat = (input - mean) / torch.sqrt(var + self._eps)
        else:
            x_hat = (input - self.running_mean) / torch.sqrt(self.running_var + self._eps)
        return self.weight * x_hat + self.bias
    # TODO END


class Dropout(nn.Module):
    # TODO START
    def __init__(self, p=0.5):
        super(Dropout, self).__init__()
        self.p = p

    def forward(self, input):
        # input: [batch_size, num_feature_map, height, width]
        if self.training:
            prob = torch.bernoulli(torch.full([input.size(i) for i in range(1, 4)], 1-self.p)).cuda()
            return prob * input / (1 - self.p)
        return input
    # TODO END


class Model(nn.Module):
    def __init__(self, drop_rate=0.5, bm=True):
        super(Model, self).__init__()
        # TODO START
        self.layer1 = nn.Sequential(
            nn.Conv2d(3, 128, 3, padding=1),
            BatchNorm2d(128) if bm else nn.Identity(),
            nn.ReLU(True),
            Dropout(drop_rate),
            nn.MaxPool2d(2, 2))
        self.layer2 = nn.Sequential(
            nn.Conv2d(128, 256, 3, padding=1),
            BatchNorm2d(256) if bm else nn.Identity(),
            nn.ReLU(True),
            Dropout(drop_rate),
            nn.MaxPool2d(2, 2))
        self.linear = nn.Sequential(
            nn.Flatten(),
            nn.Linear(16384, 10))
        # TODO END
        self.loss = nn.CrossEntropyLoss()

    def forward(self, x, y=None):
        # TODO START
        # the 10-class prediction output is named as "logits"
        x = self.layer1(x)
        x = self.layer2(x)
        logits = self.linear(x)
        # TODO END

        pred = torch.argmax(logits, 1)  # Calculate the prediction result
        if y is None:
            return pred
        loss = self.loss(logits, y)
        correct_pred = (pred.int() == y.int())
        acc = torch.mean(correct_pred.float())  # Calculate the accuracy in this mini-batch

        return loss, acc
