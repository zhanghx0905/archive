# 重力四子棋

[report](./doc/report.pdf).

### 开发环境

Windows 10 家庭中文版 1903

Visual Studio 2019 

Windows SDK 10.0.18362.0

### 文件结构

- `doc`: 报告文件夹；
- `src`: 源代码文件夹：
  - `uct.h`, `uct.cpp`: UCT类，算法主体；
  - `uctNode.hpp`: 内存池类，UCT节点类；
  - `board.h`, `board.cpp`: 棋盘类；
  - `config.h`: 全局宏定义；
