''' Some custom functions'''
import pickle
import re
from collections import defaultdict
from functools import partial
from os import listdir
from os.path import isfile, join

from hpara import *


def load_file(path, encoding):
    with open(path, 'r', encoding=encoding) as f:
        data = f.readlines()
    return data


def load_pkl(path):
    print(f'Loading {path}')
    with open(path, 'rb') as f:
        data = pickle.load(f)
    print(f'\rLoaded {path}')
    return data


def save_pkl(data, path):
    print(f'Saving {path}')
    with open(path, 'wb') as f:
        pickle.dump(data, f)
    print(f'\rSaved {path}')


def echo(input):
    return input


def get_defaultdict(defalut_value, n=2):
    '''return a n-dim (n<=2) dictionary with default values,
    based on built-in class defaultdict'''
    dic = partial(echo, defalut_value)
    for _ in range(n):
        dic = partial(defaultdict, dic)
    return dic()


def init_char_table(args):
    char_exist = get_defaultdict(False, 1)
    chars = load_file(args.file, args.encoding)[0]
    for char in chars:
        char_exist[char] = True
    save_pkl(char_exist, char_table_path)


def get_file(args, op):
    files = []
    if args.file is not None:
        files = [args.file]
    elif args.dir is not None:
        files = [
            join(args.dir, f)
            for f in listdir(args.dir)
            if isfile(join(args.dir, f)) and re.match(args.match, f) is not None
        ]
    print("Files include:")
    for f in files:
        print(f'{f}')
    return files
