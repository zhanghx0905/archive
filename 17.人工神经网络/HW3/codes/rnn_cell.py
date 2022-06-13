import torch
from torch import nn


def get_gate(input_size, hidden_size) -> nn.ModuleList:
    return nn.ModuleList([
        nn.Linear(input_size, hidden_size),
        nn.Linear(hidden_size, hidden_size)])


class RNNCell(nn.Module):
    def __init__(self, input_size, hidden_size):
        super().__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size

        self.input_layer = nn.Linear(input_size, hidden_size)
        self.hidden_layer = nn.Linear(hidden_size, hidden_size, bias=False)

    def init(self, batch_size, device):
        # return the initial state
        return torch.zeros(batch_size, self.hidden_size, device=device)

    def forward(self, incoming, state):
        # flag indicates whether the position is valid. 1 for valid, 0 for invalid.
        output = (self.input_layer(incoming) + self.hidden_layer(state)).tanh()
        new_state = output  # stored for next step
        return output, new_state


class GRUCell(nn.Module):
    def __init__(self, input_size, hidden_size):
        super().__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size

        # TODO START
        # intialize weights and layers
        self.reset_gate = get_gate(input_size, hidden_size)
        self.update_gate = get_gate(input_size, hidden_size)
        self.reset_layer = get_gate(input_size, hidden_size)
        # TODO END

    def init(self, batch_size, device):
        # TODO START
        # return the initial state
        return torch.zeros(batch_size, self.hidden_size, device=device)
        # TODO END

    def forward(self, incoming, state):
        # TODO START
        # calculate output and new_state
        r = torch.sigmoid(self.reset_gate[0](incoming) + self.reset_gate[1](state))
        z = torch.sigmoid(self.update_gate[0](incoming) + self.update_gate[1](state))
        n = torch.tanh(self.reset_layer[0](incoming) + r * self.reset_layer[1](state))
        new_state = z * state + (1 - z) * n
        output = new_state
        return output, new_state
        # TODO END


class LSTMCell(nn.Module):
    def __init__(self, input_size, hidden_size):
        super().__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size

        # TODO START
        # intialize weights and layers
        self.forget_gate = get_gate(input_size, hidden_size)
        self.info_gate = get_gate(input_size, hidden_size)
        self.output_gate = get_gate(input_size, hidden_size)
        self.gate = get_gate(input_size, hidden_size)
        # TODO END

    def init(self, batch_size, device):
        # TODO START
        # return the initial state (which can be a tuple)
        return torch.zeros(batch_size, self.hidden_size, device=device), torch.zeros(batch_size, self.hidden_size, device=device)
        # TODO END

    def forward(self, incoming, state):
        # TODO START
        # calculate output and new_state
        h, c = state
        i = torch.sigmoid(self.info_gate[0](incoming) + self.info_gate[1](h))
        f = torch.sigmoid(self.forget_gate[0](incoming) + self.forget_gate[1](h))
        g = torch.tanh(self.gate[0](incoming) + self.gate[1](h))
        o = torch.sigmoid(self.output_gate[0](incoming) + self.output_gate[1](h))
        new_c = f * c + i * g
        new_h = o * torch.tanh(new_c)
        output = new_h
        return output, (new_h, new_c)
        # TODO END
