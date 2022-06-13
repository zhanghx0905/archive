import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
from dataset import trimmed_path, bert_path, class_list, vocab_size
from pytorch_pretrained_bert import BertModel

embedding_pretrained = torch.tensor(
    np.load(trimmed_path)["embeddings"].astype('float32'))
class_num = len(class_list)


class MLP(nn.Module):
    def __init__(self, embedding_len, padding, args):
        super().__init__()
        self.fc1 = nn.Sequential(
            nn.Linear(embedding_len*padding, args['hidden_unit']),
            nn.BatchNorm1d(args['hidden_unit']),
            nn.ReLU())
        self.fc2 = nn.Linear(args['hidden_unit'], class_num)
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)

    def forward(self, texts):
        embedding = self.embedding(texts)  # batch, padding, embedding
        x = embedding.view(embedding.shape[0], -1)  # batch, padding*embedding
        x = self.fc1(x)  # batch, hidden
        x = self.fc2(x)  # batch, class
        return x


class fastText(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.ngram = args['ngram']
        assert self.ngram in [1, 2, 3]

        self.embedding = nn.Embedding(embedding_pretrained.shape[0], embedding_len)
        # self.embedding = nn.Embedding.from_pretrained(embedding_pretrained, freeze=False)
        if self.ngram >= 2:
            self.embedding_bi = nn.Embedding(vocab_size, embedding_len)
        if self.ngram == 3:
            self.embedding_tri = nn.Embedding(vocab_size, embedding_len)
        self.fc = nn.Linear(embedding_len * self.ngram, class_num)

    def forward(self, tokens):
        embedding = self.embedding(tokens[0])  # batch, padding, embedding
        if self.ngram == 2:
            embedding_bi = self.embedding_bi(tokens[1])
            embedding = torch.cat((embedding, embedding_bi), -1)
        if self.ngram == 3:
            embedding_bi = self.embedding_bi(tokens[1])
            embedding_tri = self.embedding_tri(tokens[2])
            embedding = torch.cat((embedding, embedding_bi, embedding_tri), -1)
        x = torch.mean(embedding, dim=1)
        x = self.fc(x)
        return x


class LSTM(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.lstm = nn.LSTM(input_size=embedding_len,
                            hidden_size=args['hidden'],
                            num_layers=args['num_layers'],
                            bidirectional=True,
                            batch_first=True)
        self.fc = nn.Linear(args['hidden'] * 2, class_num)
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)

    def forward(self, tokens):
        embedding = self.embedding(tokens)
        x, _ = self.lstm(embedding)
        x = self.fc(x[:, -1, :])  # bs, hidden*2
        return x


class SelfAttention(nn.Module):
    def __init__(self, hidden_dim):
        super().__init__()
        self.projection = nn.Sequential(
            nn.Linear(hidden_dim, 64),
            nn.ReLU(True),
            nn.Linear(64, 1))

    def forward(self, encoder_outputs):
        energy = self.projection(encoder_outputs)  # (B, L, H) -> (B , L, 1)
        weights = F.softmax(energy, dim=1)
        outputs = encoder_outputs * weights  # (B, L, H)
        return outputs


class LSTM_Att(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)
        self.lstm = nn.LSTM(input_size=embedding_len,
                            hidden_size=args['hidden'],
                            num_layers=args['num_layers'],
                            bidirectional=True,
                            batch_first=True)
        self.attention = SelfAttention(args['hidden'] * 2)
        self.fc = nn.Linear(args['hidden'] * 2, class_num)

    def forward(self, tokens):
        embedding = self.embedding(tokens)
        x, _ = self.lstm(embedding)
        x = self.attention(x)
        x = torch.sum(x, dim=1)
        x = self.fc(x)
        return x


class TextCNN(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.convs = nn.ModuleList([
            nn.Conv2d(1, args['out_channels'], (ks, embedding_len))
            for ks in args['kernal_size']
        ])
        self.fc = nn.Linear(args['out_channels'] *
                            len(args['kernal_size']), class_num)
        self.dropout = nn.Dropout(args['dropout'])
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)

    def conv_and_pool(self, conv_layer, x):
        x = F.relu(conv_layer(x)).squeeze(3)
        x = F.max_pool1d(x, x.size(2)).squeeze(2)
        return x

    def forward(self, tokens):
        embedding = self.embedding(tokens)
        x = embedding.unsqueeze(1)  # (bs, 1, padding, embedding)
        x = [self.conv_and_pool(conv, x)
             for conv in self.convs]  # (bs, oc) * len(ks)
        x = torch.cat(x, 1)  # (bs, oc*len(ks))
        x = self.dropout(x)
        x = self.fc(x)
        return x


class DPCNN(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.args = args
        self.conv_region = nn.Conv2d(
            1, self.args['out_channels'], (3, embedding_len), padding=(1, 0))
        self.conv2 = nn.Sequential(
            nn.ReLU(),
            nn.Conv1d(self.args['out_channels'],
                      self.args['out_channels'], 3, padding=1),
            nn.ReLU(),
            nn.Conv1d(self.args['out_channels'], self.args['out_channels'], 3, padding=1))
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)
        self.fc = nn.Linear(self.args['out_channels'], class_num)
        self.max_pool = nn.MaxPool1d(3, 2)

    def forward(self, tokens):
        embedding = self.embedding(tokens)
        x = embedding.unsqueeze(1)
        px = self.conv_region(x).squeeze(3)  # (bs, oc, len)

        x = self.conv2(px)
        x = px + x

        for _ in range(self.args['layer']):
            px = self.max_pool(x)
            x = self.conv2(px)
            x = px + x
        x = F.max_pool1d(x, x.size(2)).squeeze(2)  # (bs, oc)
        x = self.fc(x)
        return x


class TextRCNN(nn.Module):
    def __init__(self, embedding_len, args):
        super().__init__()
        self.lstm = nn.LSTM(input_size=embedding_len,
                            hidden_size=args['hidden'],
                            num_layers=args['num_layers'],
                            bidirectional=True,
                            batch_first=True)
        self.fc = nn.Linear(args['hidden'] * 2 + embedding_len, class_num)
        self.embedding = nn.Embedding.from_pretrained(
            embedding_pretrained, freeze=True)

    def forward(self, tokens):
        embedding = self.embedding(tokens)
        states, _ = self.lstm(embedding)  # bs, padding, hidden*2
        x = torch.cat((states, embedding), 2)  # bs, padding, hidden*2+embedding
        x = F.relu(x)
        x, _ = torch.max(x, dim=1)  # bs, hidden*2+embedding
        return self.fc(x)


class BERTLayer(nn.Module):
    def __init__(self, requires_grad=False):
        super().__init__()
        self.bert = BertModel.from_pretrained(bert_path)
        for param in self.bert.parameters():
            param.requires_grad = requires_grad

    def forward(self, x):
        context, mask = x[0], x[1]
        return self.bert(context, attention_mask=mask,
                         output_all_encoded_layers=False)


class BERT(nn.Module):
    def __init__(self, args):
        super().__init__()
        self.bert = BERTLayer(True)
        self.fc = nn.Linear(args['hidden'], class_num)

    def forward(self, x):
        _, x_pool = self.bert(x)
        x = self.fc(x_pool)
        return x
