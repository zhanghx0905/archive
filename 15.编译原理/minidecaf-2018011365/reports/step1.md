# Report: Step 1

2018011365 张鹤潇 

### 实验过程

Step 1需要处理只有 `return` 的程序。我使用`Antlr 4`进行词法分析和语法分析，参考实验指导书定义语法规则，

```
prog: func EOF;

func: 'int' IDENT '(' ')' '{' stmt '}';

stmt: 'return' NUM ';';
```

继承`Antlr 4`自动生成的`visitor`基类，重写各个节点类型的`visit`函数，在遍历语法树的**同时生成目标代码** (模仿Java-ANTLR的实现)。

```python
class MainVisiter(MiniDecafVisitor):
    ''' Visit Related Methods
    	省略了汇编生成相关代码 '''
    def visitProg(self, ctx: MiniDecafParser.ProgContext):
        self.visit(ctx.func())
        # 判断是否存在 main 函数

    def visitFunc(self, ctx: MiniDecafParser.FuncContext):
        # 判断函数名是否等于 main
        self.visit(ctx.stmt())

    def visitStmt(self, ctx: MiniDecafParser.StmtContext):
        self.visit(ctx.expr())

    def visitExpr(self, ctx: MiniDecafParser.ExprContext):
        ''' 判断返回值是否在 int 范围内 '''
```

### 思考题


#### 1.

输入`Lexer`不认识的字符即可。

```c
int main() { $; }
```

#### 2.

构造词法正确但语法错误的输入.

```c
void main() { return 0; }
```

#### 3.

`risc-V` 的返回值通常存储于`a0(, a1)`中.

### Acknowledgment

Antlr4 Python API借鉴了参考实现[Python-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-dzy).

`MainVisitor`类的实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

