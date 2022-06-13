# Report: Step 7

2018011365 张鹤潇 

### 实验过程

Step 7 引入作用域，需要将`symbol_table`改造成栈，栈中元素记录了每个作用域中的局部变量。
```python
# symbol_table = {}  # name : Symbol
symbol_table = []  # Stack[ {name : Symbol}, ...]
```

在进入函数或块语句`{}`时开辟作用域，`symbol_table.append({})`，退出时关闭作用域` symbol_table.pop()`.

寻找局部变量时，按作用域从内向外进行，

```python
def look_up_symbol(name: str):
    for table in reversed(symbol_table):  # 从内向外寻找
        if name in table:
            return table[name]
    return None
```

### 思考题

#### 1.

取 `int x = 0;`

在条件分支中进入 `else` 块，在此作用域中定义一个新的 `x`；`else` 块结束后其作用域弹出，`main` 函数返回的是外层定义的 `x (0)`.

#### 2.

某些语言 (TypeScript/Go) 有变量提升特性，变量可以先使用后声明。

```javascript
x = 5; // 变量 x 设置为 5

elem = document.getElementById("demo"); // 查找元素
elem.innerHTML = x;                     // 在元素中显示 x

var x; // 声明 x
```

对于有该特性的语言，最好先进行名称解析再生成IR，否则在前面的赋值语句无法得知后面的定义。

### Acknowledgment

`MainVisitor`类相关函数的实现参考了[Java-ANTLR](https://github.com/decaf-lang/minidecaf/tree/md-xxy)