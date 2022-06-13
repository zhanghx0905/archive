# README

调整如下：

- 在 `rnn_cell.py` 中添加构造 `nn.ModuleList([nn.Linear, nn.Linear])` 对象的函数。
- 在 `main.py` 中增加用 tensorboard 做数据记录的部分，添加控制 RNN 种类和 Embedding Strategy 的命令行参数。
