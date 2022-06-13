# Report: Step 6

2018011365 张鹤潇 

### 实验过程

Step 6 引入分支语句，以如下方式将 `if/else` 分支语句翻译成汇编代码，

```python
def visitIfStmt(self, ctx: MiniDecafParser.IfStmtContext):
    ''' ifStmt 语法定义：stmt: 'if' '(' expr ')' stmt ('else' stmt)? '''
    cur_No = self.get_cond_nid()				# 用于给标签编号
    
	self.visit(ctx.expr())						# 生成条件判断代码
	self.pop('t0')								# 获得条件结果
	self.write(f'\tbeqz t0, .else{cur_No}')		# 条件跳转
	self.visit(ctx.stmt(0))						# 生成 if 分支代码
	self.write(f'\tj .afterCondNo{cur_No}',		# if 分支结束
           	   f'.else{cur_No}:')
	if len(ctx.stmt()) > 1:						# 如果有 else 分支
		self.visit(ctx.stmt(1))					# 生成 else 分支代码
	self.write(f'.afterCondNo{cur_No}:')
```

三目运算符的解析方式与 `if/else` 分支语句相同，不再赘述。`visitor`函数实现简明，体现了访问者模式的威力。

### 思考题

#### 1.

强制使用大括号让程序可读性更强，同时避免了悬吊 else 问题。Python 中的强制缩进也有这样的作用。

不需要使用括号提醒开发者不要在条件表达式中进行赋值，C 语言支持在 if 括号内部的赋值操作，如`if (a = 0, b = 0, a > b, a < b, a == b)`，而 RUST 就禁止了这种语法。


### Acknowledgment

`MainVisitor`类相关函数的实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

