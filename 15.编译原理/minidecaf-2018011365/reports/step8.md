# Report: Step 8

2018011365 张鹤潇 

### 实验过程

Step 8 引入循环语句。需要实现 `for`, `while`, `do-while` 循环。添加语法定义如下：

```reStructuredText
stmt: 
	| 'while' '(' expr ')' stmt # whileStmt
	| 'for' '(' (localDel | expr? ';') expr? ';' expr? ')' stmt	# forStmt
	| 'do' stmt 'while' '(' expr ')' ';' # doStmt
	| 'break' ';' # breakStatement
    | 'continue' ';' # continueStatement
```

以 `while` 循环为例，

```python
def visitWhileStmt(self, ctx: MiniDecafParser.WhileStmtContext):
    cur_No = self.get_loop_nid()			# 循环标签
    self.write(f'.beforeLoop{cur_No}:',		# 开始循环
               f'.continueLoop{cur_No}:')	# continue 跳转到这里
    self.visit(ctx.expr())					# 条件判断
   	self.pop('t0')							# 得到条件判断结果
    self.write(f'\tbeqz t0, .afterLoop{cur_No}')	# 判定是否结束循环

    self.loops_stack.append(cur_No)			# 用栈记录当前最内层循环的编号
    self.visit(ctx.stmt())					# 访问循环体
    self.loops_stack.pop()

    self.write(f'\tj .beforeLoop{cur_No}',	# 跳转到循环开始
               f'.afterLoop{cur_No}:')		# 循环结束标签，break 跳转到这里
```

`do-while` 循环的实现与 `while` 基本一致，`for(;;)` 循环需要处理循环条件中的三个表达式，其它逻辑也与 `while` 类似，在此不再赘述。

`continue` 和 `break` 分别跳转至 `.continueLoop` 和 `.afterLoop` 即可。

```python
    def visitBreakStmt(self, ctx: MiniDecafParser.BreakStmtContext):
        if len(self.loops_stack) == 0:
            raise_error(ctx)
        self.write(f'\tj .afterLoop{self.loops_stack[-1]}')
        
    def visitContinueStmt(self, ctx: MiniDecafParser.ContinueStmtContext):
        if len(self.loops_stack) == 0:
            raise_error(ctx)
        self.write(f'\tj .continueLoop{self.loops_stack[-1]}')
```

### 思考题

#### 1.

第二种翻译方式更好。第一种实现在退出循环时需要跳转两次 (`br BEGINLOOP_LABEL`, `beqz BREAK_LABEL`)，而第二种实现则不需要这两次跳转。


### Acknowledgment

`MainVisitor`类相关函数的实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)