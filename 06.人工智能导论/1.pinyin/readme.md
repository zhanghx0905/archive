# Readme

[report](doc/report.pdf)

98/100，很奇怪为什么得不到满分。

## Usage

#### 1.下载训练好的模型[model.pkl](<https://cloud.tsinghua.edu.cn/d/dee46ef64e2f48b193af/>)，放到model文件夹下。

#### 2. 在src文件夹下运行main.py, 从stdin或文件导入数据，进行测试。

请保证运行环境有10G以上空闲内存。

```powershell
# 导入文件
python main.py --n 3 --load-model ../model/model.pkl --input-file ../data/input.txt --output-file ../data/output.txt
# 从stdin输入
python main.py --n 3 --load-model ../model/model.pkl
```

#### 3. 统计测试结果。

```powershell
python tool.py ../data/answer.txt ../data/output.txt
```

注意，程序默认输入输出的文件格式为`gbk`，如需指定编码，请用`--encoding`命令。

## 命令行帮助文件

```powershell
> python main.py --h
usage: main.py [-h] [--init-char] [--init-pinyin] [--preprocess] [--train]
               [--encoding ENCODING] [--n N] [--file FILE] [--dir DIR]
               [--match MATCH] [--load-model LOAD_MODEL]
               [--save-model SAVE_MODEL] [--input-file INPUT_FILE]
               [--output-file OUTPUT_FILE]

a Simple PinYin Input Method Based on N-gram

optional arguments:
  -h, --help            show this help message and exit
  --init-char           init char table (default: False)
  --init-pinyin         init pinyin to char dict (default: False)
  --preprocess          preprocess text (default: False)
  --train               train model (default: False)
  --encoding ENCODING   specify the encoding of input file (default: gbk)
  --n N                 select n gram model, where 2 <= n <= 4 (default: 2)
  --file FILE           file path for training data (default: None)
  --dir DIR             dir path for training data (default: None)
  --match MATCH         use regex to match files (default: .*)
  --load-model LOAD_MODEL
                        path to load model (default: None)
  --save-model SAVE_MODEL
                        path to save model (default: ../model/model.pkl)
  --input-file INPUT_FILE
                        the path of input file (default: None)
  --output-file OUTPUT_FILE
                        the path of output file (default: None)
```


## 文件结构

- `src`: 源代码文件夹.
  - `main.py`: 程序入口；
  - `hpara.py`: 文件路径，超参数；
  - `func.py`: 与文件相关的函数；
  - `train.py`: 语料预处理与训练；
  - `predict.py`: HMM主算法；
  - `tool.py`: 统计字准确率和句准确率的工具.

- `model`: 模型文件夹.
  - `char-table.pkl`：汉字表；
  - `char2yinpin.pkl`：汉字到拼音的字典；
  - `pinyin2char.pkl`：拼音到汉字的字典；
  - `model.pkl`：n-gram模型.
- `data`.

## 附录：复现步骤

获取[新浪新闻语料和汉字、拼音表](https://cloud.tsinghua.edu.cn/d/d97edd5fc60947d480cf/)。解压到`data`文件夹下。

#### 1. 初始化拼音表，汉字表

该步生成的文件已经保存于`./model`文件夹下。

```powershell
python main.py --init-char --file ../data/一二级汉字表.txt
python main.py --init-pinyin --file ../data/拼音汉字表.txt
```

#### 2. 训练语料预处理

预处理可能会非常耗时。

```
python main.py --preprocess --dir ../data/ --match "^2016"
```

#### 3. 训练

以训练三元字模型为例。

```
python main.py --train --dir ../data/ --match "^2016" --n 3
```

也可以在已有模型上继续训练，控制`--load-model`参数即可.

#### 4. 测试

见Usage部分。
