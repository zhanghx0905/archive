# Report: Step 5

2018011365 张鹤潇 

### 实验过程

Step 5 引入局部变量，定义`Symbol`类表示局部变量，`symbol_table`表存储局部变量。

```python
# 局部变量类
Symbol = namedtuple('Symbol', 'offset ty')
symbol_table = {}  # name(str): value(Symbol)
```

在汇编层面，需要操作栈指针和相关寄存器为局部变量开辟栈空间，`visitFunc`实现如下，

```python
def visitFunc(self, ctx: MiniDecafParser.FuncContext):
    # ...
    
	self.write("\t.text",
            	f"\t.global {self.cur_func}",
            	f"{self.cur_func}:")
    # 保存 ra, fp, 设置fp = sp
	self.push('ra', 'fp')
	self.write('\tmv fp, sp')
    
    # 访问函数体并统计局部变量数
    self.local_var_cnt = 0
    for item in ctx.blockItem():
    	self.visit(item)
    
    # 为临时变量开辟栈空间
    self.asm.insert(backtrace_pos, f'\taddi sp, sp, {-4*self.local_var_cnt}')
    
    # 读取返回值，恢复 ra, fp, sp
    self.write(f'.exit.{self.cur_func}:',
               '\tlw a0, 0(sp)',
               '\tmv sp, fp')
    self.pop('fp', 'ra')
    self.write('\tret')
```

访问函数体时，以 `lw t0, var.offset(fp)` 的形式读写局部变量 `var`.

声明、使用变量，或对变量赋值时，需要检查变量是否已经存在。

```python
# 变量使用
if name not in symbol_table:
	raise_error("use not defined variable", ctx)
# 不允许重定义
if name in symbol_table:
    raise_error('try declaring a declared variable', ctx)
```

### 思考题

#### 1.

   从高地址到低地址：

   - callee-saved register：如果使用被调用者负责保存的寄存器，需要在这里保存。一般来说至少有 `fp, ra`，需要8 bytes。
   - 局部变量：函数体内部定义的变量。可以为空（没有局部变量或者局部变量都存在寄存器中）。
   - 运算栈：进行运算时保存的临时结果，包括返回值。可以为空。
   - 子函数参数：调用子函数时在寄存器中存不下的参数 (>8)。可以为空。

#### 2.

定义变量时，不再需要检测变量是否已经被定义过；查找变量的逻辑不需要修改。

### Acknowledgment

`MainVisitor`类相关函数的实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

