import numpy as np


class Layer(object):
    def __init__(self, name, trainable=False):
        self.name = name
        self.trainable = trainable
        self._saved_tensor = None

    def forward(self, input):
        pass

    def backward(self, grad_output):
        pass

    def update(self, config):
        pass

    def _saved_for_backward(self, tensor):
        '''The intermediate results computed during forward stage
        can be saved and reused for backward, for saving computation'''

        self._saved_tensor = tensor


class Relu(Layer):
    def __init__(self, name):
        super(Relu, self).__init__(name)

    def forward(self, input):
        # TODO START
        '''Your codes here'''
        mask = input > 0
        self._saved_for_backward(mask)
        return mask * input
        # TODO END

    def backward(self, grad_output):
        # TODO START
        '''Your codes here'''
        return self._saved_tensor * grad_output
        # TODO END


class Sigmoid(Layer):
    def __init__(self, name):
        super(Sigmoid, self).__init__(name)

    def forward(self, input):
        # TODO START
        '''Your codes here'''
        res = 1 / (1 + np.exp(-input))
        self._saved_for_backward(res)
        return res
        # TODO END

    def backward(self, grad_output):
        # TODO START
        '''Your codes here'''
        res = self._saved_tensor
        return grad_output * res * (1 - res)
        # TODO END


class Gelu(Layer):
    def __init__(self, name):
        super(Gelu, self).__init__(name)

    def forward(self, input):
        # TODO START
        '''Your codes here'''
        self._saved_for_backward(input)
        res = 0.5 * input * (1 + np.tanh(np.sqrt(2 / np.pi)
                                         * (input + 0.044715 * np.power(input, 3))))
        return res
        # TODO END

    def backward(self, grad_output):
        # TODO START
        '''Your codes here'''
        u = self._saved_tensor
        tmp = 0.0356774 * np.power(u, 3) + 0.797885 * u
        grad = 0.5 * np.tanh(tmp) + 0.5 + (0.0535161*np.power(u, 3) + 0.398942*u) * 1/np.square(np.cosh(tmp))
        return grad_output * grad
        # TODO END


class Linear(Layer):
    def __init__(self, name, in_num, out_num, init_std):
        super(Linear, self).__init__(name, trainable=True)
        self.in_num = in_num
        self.out_num = out_num
        self.W = np.random.randn(in_num, out_num) * init_std
        self.b = np.zeros(out_num)

        self.grad_W = np.zeros((in_num, out_num))
        self.grad_b = np.zeros(out_num)

        self.diff_W = np.zeros((in_num, out_num))
        self.diff_b = np.zeros(out_num)

    def forward(self, input):
        # TODO START
        '''Your codes here'''
        self._saved_for_backward(input)
        return np.dot(input, self.W) + self.b
        # TODO END

    def backward(self, grad_output):
        # TODO START
        '''Your codes here'''
        input = self._saved_tensor
        self.grad_b = np.sum(grad_output, axis=0)
        self.grad_W = np.dot(input.T, grad_output)
        return np.dot(grad_output, self.W.T)
        # TODO END

    def update(self, config):
        mm = config['momentum']
        lr = config['learning_rate']
        wd = config['weight_decay']

        self.diff_W = mm * self.diff_W + (self.grad_W + wd * self.W)
        self.W = self.W - lr * self.diff_W

        self.diff_b = mm * self.diff_b + (self.grad_b + wd * self.b)
        self.b = self.b - lr * self.diff_b


if __name__ == '__main__':
    ''' 简单的梯度检查 '''
    gelu = Gelu('test')
    test_in = np.random.random((5, 5))

    sigma = 1e-5
    fin = gelu.forward(test_in)
    fin_sigma = gelu.forward(test_in + sigma)
    grad_test = (fin_sigma - fin) / sigma

    grad_out = gelu.backward(np.ones((5, 5)))
    print(grad_test)
    print(grad_out)
