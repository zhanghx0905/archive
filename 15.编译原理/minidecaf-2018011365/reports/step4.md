# Report: Step 4

2018011365 张鹤潇 

### 实验过程

Step 4 增加了比较和逻辑表达式，需要处理`<,>,<=,>=,!=,==,&&,||`等二元运算符；

在 Step 3 的基础上扩充`BIN_TO_ASM`，并实现相应的`visit`函数即可。

```python
BIN_TO_ASM = {	# 省略了 step3.md 中提到的 + - * / %
              '||': '\tsnez t0, t0\n'
                    '\tsnez t1, t1\n'
                    '\tor t0, t0, t1',
              '&&': '\tsnez t0, t0\n'
                    '\tsnez t1, t1\n'
                    '\tand t0, t0, t1',
              '==': '\tsub t0, t0, t1\n'
                    '\tseqz t0, t0',
              '!=': '\tsub t0, t0, t1\n'
                    '\tsnez t0, t0',
              '<': '\tslt t0, t0, t1',
              '<=': '\tsgt t0, t0, t1\n'
                    '\txori t0, t0, 1',
              '>': '\tsgt t0, t0, t1',
              '>=': '\tslt t0, t0, t1\n'
                    '\txori t0, t0, 1', }
```

### 思考题


#### 1.

不一定，在实现了短路求值的情况下，表达式 `1||(3+2)` 就不需要计算`3+2`的值，但是本实验并没有这种需求，我也没有实现。

### 2.

短路求值使编程者能更精细的控制程序行为，列举两个常见应用场景：

- 利用短路特性判空，

  ```python
  if a is not None and a.do_something():
      pass
  ```

- 将可能触发短路特性的条件前置，以降低计算量，

  ```python
  # 若 likely() == True, 则不需计算 unlikely()
  if likely() or unlikely():
      pass
  
  # 若 unlikely() == False, 则不需计算 likely()
  if unlikely() and likely():
      pass
  ```


### Acknowledgment

在 Step 3 的基础上自主完成。

