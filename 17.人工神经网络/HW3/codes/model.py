import torch
from torch import nn
import torch.nn.functional as F

from rnn_cell import RNNCell, GRUCell, LSTMCell


class RNN(nn.Module):
    def __init__(self,
                 num_embed_units,  # pretrained wordvec size
                 num_units,        # RNN units size
                 num_layers,       # number of RNN layers
                 num_vocabs,       # vocabulary size
                 wordvec,          # pretrained wordvec matrix
                 dataloader,       # dataloader
                 ty: str,
                 embedding: str):

        super().__init__()

        # load pretrained wordvec
        self.wordvec = wordvec
        # the dataloader
        self.dataloader = dataloader

        # TODO START
        # fill the parameter for multi-layer RNN
        CELLS = {'rnn': RNNCell, 'gru': GRUCell, 'lstm': LSTMCell}
        Cell = CELLS[ty]
        self.cells = nn.Sequential(
            Cell(num_embed_units, num_units),
            *[Cell(num_units, num_units) for _ in range(num_layers - 1)]
        )
        if embedding == 'random':
            self.embedding = nn.Embedding(num_vocabs, num_embed_units)
        else:
            self.embedding = nn.Embedding.from_pretrained(wordvec, freeze=(embedding == 'static'))
        # TODO END

        # intialize other layers
        self.linear = nn.Linear(num_units, num_vocabs)

    def forward(self, batched_data, device):
        # Padded Sentences
        sent = torch.tensor(batched_data["sent"], dtype=torch.long, device=device)  # shape: (batch_size, length)
        # An example:
        #   [
        #   [2, 4, 5, 6, 3, 0],   # first sentence: <go> how are you <eos> <pad>
        #   [2, 7, 3, 0, 0, 0],   # second sentence:  <go> hello <eos> <pad> <pad> <pad>
        #   [2, 7, 8, 1, 1, 3]    # third sentence: <go> hello i <unk> <unk> <eos>
        #   ]
        # You can use self.dataloader.convert_ids_to_sentence(sent[0]) to translate the first sentence to string in this batch.

        # Sentence Lengths
        length = torch.tensor(batched_data["sent_length"], dtype=torch.long, device=device)  # shape: (batch)
        # An example (corresponding to the above 3 sentences):
        #   [5, 3, 6]

        batch_size, seqlen = sent.shape

        # TODO START
        # implement embedding layer
        embedding = self.embedding(sent)  # shape: (batch_size, length, num_embed_units)
        # TODO END

        now_state = []
        for cell in self.cells:
            now_state.append(cell.init(batch_size, device))

        loss = 0
        logits_per_step = []
        for i in range(seqlen - 1):
            hidden = embedding[:, i]
            for j, cell in enumerate(self.cells):
                hidden, now_state[j] = cell(hidden, now_state[j])  # shape: (batch_size, num_units)
            logits = self.linear(hidden)  # shape: (batch_size, num_vocabs)
            logits_per_step.append(logits)

        # TODO START
        # calculate loss
        logits_per_step = torch.stack(logits_per_step, dim=1)  # (batch_size, seqlen - 1, num_vocabs)
        logits = F.log_softmax(logits_per_step, -1)
        logits = logits.gather(2, sent[:, 1:].unsqueeze(-1)).squeeze()  # (batch_size, seqlen - 1)

        length -= 1  # remove <go>
        for idx in range(seqlen - 1):   # 有没有不用循环的方法?
            logits[:, idx] *= (idx < length)
        loss = -torch.mean(torch.sum(logits, dim=-1) / length)
        # TODO END

        return loss, logits_per_step

    def inference(self, batch_size, device, decode_strategy, temperature, max_probability):
        # First Tokens is <go>
        now_token = torch.tensor([self.dataloader.go_id] * batch_size, dtype=torch.long, device=device)
        flag = torch.tensor([1] * batch_size, dtype=torch.float, device=device)

        now_state = []
        for cell in self.cells:
            now_state.append(cell.init(batch_size, device))

        generated_tokens = []
        for _ in range(50):  # max sentecne length

            # TODO START
            # translate now_token to embedding
            embedding = self.embedding(now_token)  # shape: (batch_size, num_embed_units)
            # TODO END

            hidden = embedding
            for j, cell in enumerate(self.cells):
                hidden, now_state[j] = cell(hidden, now_state[j])
            logits = self.linear(hidden)  # shape: (batch_size, num_vocabs)

            if decode_strategy == "random":
                prob = (logits / temperature).softmax(dim=-1)  # shape: (batch_size, num_vocabs)
                now_token = torch.multinomial(prob, 1)[:, 0]  # shape: (batch_size)
            elif decode_strategy == "top-p":
                # TODO START
                # Reference: https://gist.github.com/thomwolf/1a5a29f6962089e871b94cbd09daf317
                # implement top-p samplings
                prob = (logits / temperature).softmax(dim=-1)
                sorted_logits, sorted_indices = torch.sort(prob, descending=True)
                cumulative_probs = torch.cumsum(sorted_logits, dim=-1)

                # Remove tokens with cumulative probability above the threshold
                sorted_indices_to_remove = cumulative_probs >= max_probability
                # Shift the indices to the right to keep also the first token above the threshold
                sorted_indices_to_remove[..., 1:] = sorted_indices_to_remove[..., :-1].clone()
                sorted_indices_to_remove[..., 0] = 0

                indices_to_remove = torch.zeros_like(prob, dtype=torch.bool).scatter_(dim=-1, index=sorted_indices, src=sorted_indices_to_remove)

                prob[indices_to_remove] = 0
                prob /= prob.sum(-1, keepdim=True)
                now_token = torch.multinomial(prob, 1)[:, 0]  # shape: (batch_size)
                # TODO END
            else:
                raise NotImplementedError("unknown decode strategy")

            generated_tokens.append(now_token)
            flag = flag * (now_token != self.dataloader.eos_id)

            if flag.sum().tolist() == 0:  # all sequences has generated the <eos> token
                break

        return torch.stack(generated_tokens, dim=1).detach().cpu().numpy()
