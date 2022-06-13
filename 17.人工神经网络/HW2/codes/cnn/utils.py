from collections import namedtuple
from torch.utils.tensorboard.writer import SummaryWriter


class Stat:
    def __init__(self, name, writer: SummaryWriter):
        self.step = 0
        self.name = name
        self.writer = writer

    def log(self, loss, acc):
        self.step += 1
        self.writer.add_scalar(f'Loss/{self.name}', loss, self.step)
        self.writer.add_scalar(f'Acc/{self.name}', acc, self.step)


Result = namedtuple('Result', 'val_acc val_loss train_acc train_loss')
Result.__str__ = lambda self: (f'Train Acc:  {self.train_acc:.2%}\n'
                               f'Train Loss: {self.train_loss:.5f}\n'
                               f'Val Acc:    {self.val_acc:.2%}\n'
                               f'Val Loss:   {self.val_loss:.5f}')
