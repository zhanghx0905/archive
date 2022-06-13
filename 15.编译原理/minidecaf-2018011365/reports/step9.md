# Report: Step 9

2018011365 张鹤潇 

### 实验过程

Step 9 引入函数，需要解析函数的声明、定义和调用。增加语法定义如下，

```shell
func:
	ty IDENT '(' (ty IDENT (',' ty IDENT)*)? ')' '{' blockItem* '}'	# definedFunc
	| ty IDENT '(' (ty IDENT (',' ty IDENT)*)? ')' ';'	# declaredFunc
	;
postfix:
	IDENT '(' (expr (',' expr)*)? ')' # callPostfix
    # ...
    ;
```

将函数的类型信息存储在全局字典中，

```python
# name: FunType
declared_fun_table = {}	# 声明过的函数表
defined_fun_table = {}	# 定义过的函数表

class FunType:
    ''' 存储函数返回值和参数类型 '''
    def __init__(self,
                 ret_type: MiniType,
                 para_types: List[MiniType]) -> None:
        self.ret_type = ret_type
        self.para_types = tuple(para_types)

    def __eq__(self, o: object) -> bool:
        if self.ret_type != o.ret_type or self.para_types != o.para_types:
            return False
        return True
```

声明函数时，需要将其类型信息存储到`declared_fun_table`中；调用函数时，要检查传入的参数类型是否与`declared_fun_table`存储的类型信息一致。

以最复杂的函数定义为例，其`visit`函数大致实现如下，略去了部分错误处理的代码，

```python
 	def visitDefinedFunc(self, ctx: MiniDecafParser.DefinedFuncContext):
        # 生成函数头
        name = ctx.IDENT(0).getText()
        self.write("\t.text",
                   f"\t.global {name}",
                   f"{name}:")

        # 返回值和参数类型检查
        ret_type = self.visit(ctx.ty(0))
        para_types = [self.visit(ctx.ty(i)) for i in range(1, len(ctx.ty()))]
        fun_type = FunType(ret_type, para_types)
        if name in declared_fun_table and declared_fun_table[name] != fun_type:
            raise_error(ctx)
        declared_fun_table[name] = fun_type
        defined_fun_table[name] = fun_type
		
        # 保存返回地址和栈指针
        self.push('ra', 'fp')
        self.write('\tmv fp, sp')
        
        # 在生成函数体汇编码时统计局部变量的个数
        # 以决定分配多少栈空间
        self.local_var_cnt = 0
        backtrace_pos = len(self.asm)
		
		# 将参数加入函数体作用域
        symbol_table.append({})	# 开辟作用域
        for idx in range(1, len(ctx.IDENT())):
            para_name = ctx.IDENT(idx).getText()
            if idx < 9:	# 多于8个参数，在栈中保存
                self.local_var_cnt += 1
                self.write(f'\tsw a{idx-1}, {-4*idx}(fp)')
                # Symbol(offset, type)
                symbol = Symbol(-4 * idx, fun_type.para_types[idx - 1])
            else:
                symbol = Symbol(4 * (idx - 9 + 2), fun_type.para_types[idx - 1])
            symbol_table[-1][para_name] = symbol
         
        # 生成函数体汇编码
        for item in ctx.blockItem():
            self.visit(item)
        symbol_table.pop()
		
        # 已经知道了局部变量数，回填汇编代码
        # 开辟栈空间
        self.asm.insert(backtrace_pos, f'\taddi sp, sp, {-4*self.local_var_cnt}')
		
        # 恢复返回地址和栈指针
        self.write(f'.exit.{name}:',
                   '\tlw a0, 0(sp)',
                   '\tmv sp, fp')
        self.pop('fp', 'ra')
        self.write('\tret')

        return NoType()
```

### 思考题

#### 1.

一个例子如下：

```c
int num = 0;

int increment() {
    num = num + 1;
    return num;
}

int minus(int a, int b) {
    return a - b;
}

int main() {
    return minus(num, increment());
}
```

在这个例子中，调用 `increment` 函数会改变全局变量 `num` 的值。对于语句`minus(num, increment())`，不同的求值顺序将带来不同的结果。如果从左到右求值，则传入参数`a=0,b=1`, 返回值为-1；而如果从右到左求值，则会传入参数`a=b=1`，返回值为0.

全局变量作为 MiniDecaf 的一部分，在 Step 10 中实现。


### Acknowledgment

相关实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

