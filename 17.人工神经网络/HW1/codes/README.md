# README

通过命令行参数控制网络模型。

```shell
usage: run_mlp.py [-h] [--layers LAYERS] [--loss LOSS] [--acti ACTI]
                  [--verbose]

optional arguments:
  -h, --help       show this help message and exit
  --layers LAYERS  1 or 2
  --loss LOSS      one of l2, entropy and hinge
  --acti ACTI      one of sigmoid, relu and gelu
  --verbose
```

Loss/Acc曲线通过 `Tensorboard` 以文件的形式保存在 `code/log` 目录下，可通过

```shell
tensorboard --logdir=./log
```

查看。

### 对原框架的改动

在保留原框架绝大部分代码的基础上做了如下改动。

- 在 `layers.py` 中增加了 `Gelu` 梯度检查的代码，直接运行 `layers.py` 即可触发。
- 在`solve_net.py`中修改了训练和测试函数，使之返回`Loss/Acc`数据。
- 在`run_mlp.py`中增加了命令行解析，`Early Stoping`，`Loss/Acc`记录的功能。