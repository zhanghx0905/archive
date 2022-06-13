# 中文长文本情感分类

[下载](https://cloud.tsinghua.edu.cn/d/cdbd9691f68f4fa78200/)完整版程序(包含训练好的模型文件，bert预训练模型和TextGCN相关文件)。

[实验报告](./doc/report.pdf)


## 测试方法

在src文件夹下运行：

```shell
sh test.sh
```

对于TextGCN，在text_gcn文件夹下运行:

```shell
python train.py r8
```

## 文件结构

`src`: 源代码文件夹.

- `run.py`: 程序入口；
- `model.py`: 定义了各种神经网络模型；
- `dataset.py`: 数据处理；
- `train_eval.py`: 神经网络训练；
- `run_ml.py`: 朴素贝叶斯等ML模型。

`data`: 数据文件夹，同时存放BERT预训练模型。

`model`: 保存训练好的模型。

`config`: 模型超参数。

`gcn`: TextGCN相关文件。

## 复现指南

下载[预训练词向量](https://pan.baidu.com/s/1svFOwFBKnnlsqrF1t99Lnw)，解压到data文件夹下，运行`dataset.py`生成词向量表。

```shell
python dataset.py
```

修改`config`文件夹下各个模型对应的超参数，将`load`设置为0，将`num_epochs`设定为需要的epoch数，运行`test.sh`中命令进行训练。

| Model                 | 推荐epoch |
| --------------------- | ------- |
| MLP                   | 50        |
| fastText(bow)         | 60        |
| fastText(bigram)      | 60        |
| TextCNN               | 150       |
| DPCNN                 | 50        |
| BiLSTM                | 10        |
| BiLSTM with Attention | 10        |
| TextRCNN              | 20        |
| BERT                   | 10        |

对于TextGCN，保证预训练词向量在`data`文件夹下，运行`train.sh`即可完成训练。