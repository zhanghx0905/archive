# -*- coding: utf-8 -*-
"""
Created on 2018.10.03

将中缀表达式转换成逆波兰式。

input: p+q*r
output: 对应逆波兰表达式: pqr*+

@author: zhanghx0905
"""

operands = [chr(i) for i in range(ord('a'), ord('z') + 1)]
operators = {'*': 3, '/': 3, '+': 1, '-': 1, '#': -1, "(": -1}  # 使用字典类型规定运算符的优先级


def transform(source: str) -> str:
    result, stack = [], ['#']  # 创建一个result栈和一个中间栈
    source = source.lower()
    for j in source:
        # 若取出的是原子命题，则直接送入result栈
        if j in operands:
            result.append(j)
        # 若取出的是 (，则直接送入中间栈顶
        elif j == '(':
            stack.append(j)
        # 若取出的是 )，则将中间栈顶与距栈顶最近的 ( 之间的运算符逐个出栈
        # 送入 result 栈，并取出 (
        elif j == ')':
            for x in range(len(stack)):
                if stack[-1] == '(':
                    stack.pop()
                    break
                else:
                    result.append(stack.pop())
        # 若取出是运算符，则将其与中间栈元素自顶向下逐个比较，如果其优先级大于等于栈顶运算符优先级，
        # 则将该运算符送进中间栈；否则，将中间栈的栈顶运算符取出，送入result栈中。
        elif j in operators.keys():
            for _ in range(len(stack)):
                if operators[j] >= operators[stack[-1]]:
                    stack.append(j)
                    break
                else:
                    result.append(stack.pop())
        else:
            raise SyntaxError()
    for _ in range(len(stack) - 1):
        result.append(stack.pop())
    return ''.join(result)


if __name__ == '__main__':
    source = input("请输入合乎规范的中缀运算式:\n")
    try:
        result = transform(source)
    except SyntaxError:
        print("语法错误")
    else:
        print(f"对应逆波兰表达式: {result}")
