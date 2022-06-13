# Report: Step 11

2018011365 张鹤潇 

### 实验过程

Step 11 引入左值的概念，增加指针类型和相关运算，增加显式类型转换语法。

参考[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)，将左值引入类型系统：定义 Minidecaf 所有类型的基类，在基类中记录该类型是左值还是右值。

```python
ValueCat = Enum('ValueCat', 'LVALUE RVALUE')	# 左值 or 右值

class MiniType:
    def __init__(self, name, vcat=ValueCat.RVALUE) -> None:
        self.name = name
        self.vcat = vcat

    def __str__(self) -> str:
        return f'{self.name}({self.vcat})'

    def __eq__(self, o: object) -> bool:
        return isinstance(o, self.__class__)

    def referenced(self):	# 取地址
        raise UnsupportedOperation(f"{self.name} can't be referenced.")

    def dereferenced(self):	# 解引用
        raise UnsupportedOperation(f"{self.name} can't be referenced.")

    def value_cast(self, target_cat):	# 强制类型转换
        return self
```

继承该基类，实现`IntType`, `PointerType`, `FunType` 三个子类，分别表示整形、指针和函数。以指针类型`PointerType` 为例，实现如下：

```python
class PointerType(MiniType):
    def __init__(self, star_cnt, vcat=ValueCat.RVALUE) -> None:
        super().__init__(f'PointerType<{star_cnt}>', vcat)
        self.star_cnt = star_cnt	# 指针的嵌套层数

    def __len__(self):
        return 4

    def __eq__(self, o: object) -> bool:
        return isinstance(o, PointerType) and \
            self.star_cnt == o.star_cnt and \
            o.vcat is self.vcat

    def referenced(self):
        if self.vcat == ValueCat.LVALUE:
            return PointerType(self.star_cnt+1)
        raise UnsupportedOperation("Rvalue can't be referenced.")

    def dereferenced(self):
        if self.star_cnt > 1:
            return PointerType(self.star_cnt-1, ValueCat.LVALUE)
        return IntType(ValueCat.LVALUE)

    def value_cast(self, target_cat):
        return PointerType(self.star_cnt, target_cat)
```

令 `MainVisitor` 类的每个访问者函数都返回其访问表达式的类型，同时增加对右值和左值的处理。以实现了取址(&)和解引用(*)的一元运算函数 `visitOpUnary` 为例，

```python
  	def visitOpUnary(self, ctx: MiniDecafParser.OpUnaryContext):
        ty = self.visit(ctx.unary())
        op = ctx.getChild(0).getText()

        if op == '*':	# 转为右值 解引用
            return self.rcast(ty, ctx).dereferenced()
        elif op == '&':	# 返回指针类型
            return ty.referenced()
        else:	# 作用于 int 数据的一元算符 (- ！~)
            # ...
            return IntType()
```

类型转换语句 `'(' ty ')' unary` 的解析相对简单，在此不再赘述。

### 思考题

### 1.

名称解析完成后，才能知道每个变量和表达式的类型，这是类型检查所必需的。

### 2.

在 C++ 中，允许编程者将一个表达式赋给多种类型的变量，如下例：

```cpp
    int i = 'a';
    char c = 'a';
    cout << i << ' ' << c; // 97 a
```

### 3.

如果要实现指针大小比较，需要检查被比较的两个指针类型是否相同；相同类型的指针才能比较大小。

比较的方法同无符号整形数，地址高的指针较大。


### Acknowledgment

相关实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)

