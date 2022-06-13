# Report: Step 2

2018011365 张鹤潇 

### 实验过程

Step 2需要处理单目运算符，单目运算汇编代码生成的统一框架如下。

将栈顶存储的变量读出到寄存器中，进行单目运算后，存回栈中。

```python
    # do unary operations
	op = ctx.getChild(0).getText()
	self.pop('t0')
    self.write(f'\t{UNA_TO_ASM[op]} t0, t0')
    self.push('t0')
```

`UNA_TO_ASM`是从单目操作符到汇编代码的映射，

```python
	UNA_TO_ASM = {'-': 'neg', '!': 'seqz', '~': 'not'}
```

### 思考题


#### 1.

$a = 2^{31} - 1$

$-\sim a = 2^{31}$, 越界


### Acknowledgment

在 Step 1 的基础上自主完成。

