# Report: Step 3

2018011365 张鹤潇 

### 实验过程

Step 3需要处理两目运算符，包括`+,-,*,/,%`.

两目运算目标代码生成的统一框架如下，

```python
    def do_binary_op(self, op):
        self.pop('t1', 't0')
        self.write(BIN_TO_ASM[op])
        self.push('t0')
```

`BIN_TO_ASM`是从两目操作符到汇编代码的映射，

```python
BIN_TO_ASM = {'+': '\tadd t0, t0, t1',
              '-': '\tsub t0, t0, t1',
              '*': '\tmul t0, t0, t1',
              '/': '\tdiv t0, t0, t1',
              '%': '\trem t0, t0, t1',
              # 在 Step 4 中将添加关系运算和逻辑运算符
             }
```

在此基础上补充 `g4` 文件，并实现相应的 `visit` 函数即可。

### 思考题


#### 1.

```assembly
# t0 压栈
addi sp, sp, -4
sw t0, 0(sp)

# 弹栈到 t0
lw t0, 0(sp)
addi sp, sp, 4
```

#### 2.

以下代码构成了未定义行为：

```c
#include <stdio.h>

int main() {
  int a = -0x80000000;	// 左操作数
  int b = -1;	// 右操作数
  printf("%d\n", a / b);
  return 0;
}
```

在 x86-64（Ubuntu 18.04  gcc）下编译运行，

```reStructuredText
Floating point exception (core dumped)
```

在RISCV-32 的 qemu 模拟器中编译运行，

```
-2147483648
```

### Acknowledgment

在 Step 2 的基础上自主完成。

