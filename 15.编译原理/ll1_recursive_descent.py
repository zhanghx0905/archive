
hint = '''
输入字符串，检验其是否符合 CFG:
    E -> [F]A
    A -> E | epsilon
    F -> aB
    B -> aB | epsilon
'''
s = input(hint) + '#'
iter = (c for c in s)
lookahead, cnt = next(iter), 0


def match_token(excepted: str):
    global lookahead, cnt
    if lookahead != excepted:
        raise SyntaxError('syntax error')
    lookahead = next(iter)
    cnt += 1


def ParseE():
    match_token('[')
    ParseF()
    match_token(']')
    ParseA()


def ParseA():
    if lookahead in ['[']:
        ParseE()
    elif lookahead in ['#']:
        pass
    else:
        raise SyntaxError('syntax error')


def ParseF():
    match_token('a')
    ParseB()


def ParseB():
    if lookahead in ['a']:
        match_token('a')
        ParseB()
    elif lookahead in [']']:
        pass
    else:
        raise SyntaxError('syntax error')


try:
    ParseE()
except SyntaxError:
    print(f'{cnt}:{lookahead} 处有语法错误！')
else:
    print('匹配成功！')
