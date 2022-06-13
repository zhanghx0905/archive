from __future__ import division
import numpy as np


class EuclideanLoss(object):
    def __init__(self, name):
        self.name = name

    def forward(self, input, target):
        # TODO START
        '''Your codes here'''
        return np.linalg.norm(input - target) / (2*input.shape[0])
        # TODO END

    def backward(self, input, target):
        # TODO START
        '''Your codes here'''
        return (input-target)/input.shape[0]
        # TODO END


class SoftmaxCrossEntropyLoss(object):
    def __init__(self, name):
        self.name = name
        self.epsilon = 1e-40

    def forward(self, input, target):
        # TODO START
        '''Your codes here'''
        exp = np.exp(input - np.max(input))
        prob = exp / (np.sum(exp, axis=1, keepdims=True) + self.epsilon)
        return np.mean(np.sum(- target * np.log(prob + self.epsilon), axis=1))
        # TODO END

    def backward(self, input, target):
        # TODO START
        '''Your codes here'''
        exp = np.exp(input - np.max(input))
        prob = exp / (np.sum(exp, axis=1, keepdims=True) + self.epsilon)
        return (prob - target) / input.shape[0]
        # TODO END


class HingeLoss(object):
    def __init__(self, name, threshold=5):
        self.name = name
        self.threshold = threshold

    def forward(self, input, target):
        # TODO START
        '''Your codes here'''
        pred_score = np.sum(target*input, axis=1, keepdims=True)
        mask = np.logical_not(target == 1)  # 将k=t置零
        loss_mat = np.maximum(0, self.threshold - pred_score + input) * mask
        return np.mean(np.sum(loss_mat, axis=1))
        # TODO END

    def backward(self, input, target):
        # TODO START
        '''Your codes here'''
        pred_score = np.sum(target*input, axis=1, keepdims=True)
        mask = np.logical_not(target == 1)
        grad = (self.threshold - pred_score + input > 0) * mask
        grad = (grad-np.sum(grad, axis=1, keepdims=True)
                * target)/input.shape[0]
        return grad
        # TODO END
