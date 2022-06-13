# Report: Step 10

2018011365 张鹤潇 

### 实验过程

Step 10 引入全局变量。增加全局变量表，存储被声明过的和已初始化的全局变量类型信息。

```python
# name : Type
declared_global_table = {}
inited_global_table = {}
```

全局变量赋值语句的`visit`函数实现如下，略去了部分代码细节。

```python
    def visitGlobalIntOrPointerDecl(self, ctx):
        name = ctx.IDENT().getText()
        ty = self.visit(ctx.ty())
        # 检查多次声明的全局变量类型是否相同
        if name in declared_global_table and declared_global_table[name] != ty:
            raise_error(ctx)

        declared_global_table[name] = ty
        if (num := ctx.NUM()) is not None:
            # 全局变量只能被初始化一次
            if name in inited_global_table:
                raise_error(ctx)
            inited_global_table[name] = ty.value_cast(ValueCat.RVALUE)

            self.write('\t.data',
                       '\t.align 4',
                       f'{name}:',
                       f'\t.word {num.getText()}')
```

对于未初始化的全局变量，要将其存储到 `bss` 段中。

解析表达式时，对全局变量处理如下，

```python
	# ...	
    elif name in declared_global_table:
        # 读取全局变量, 存入栈中
        self.write(f'\tlui t0, %hi({name})',
                   f'\taddi t0, t0, %lo({name})')
        self.push('t0')
```

### 思考题

#### 1.

将全局变量 `a` 的值读到寄存器 `t0` 所需的 riscv 指令序列：

```assembly
lui t0, %hi(a)
addi t0, t0, %lo(a)
li t0, 0(t0)
```


### Acknowledgment

相关实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

