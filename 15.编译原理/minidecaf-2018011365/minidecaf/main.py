import argparse

from antlr4 import FileStream
from antlr4.CommonTokenStream import CommonTokenStream
from antlr4.error.ErrorStrategy import BailErrorStrategy

from .generated.MiniDecafLexer import MiniDecafLexer
from .generated.MiniDecafParser import MiniDecafParser
from .visitor import MainVisitor


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("instream", type=str)
    parser.add_argument("out", type=str, nargs="?")
    return parser.parse_args()


def main():
    args = parse_args()
    instream = FileStream(args.instream)
    lexer = MiniDecafLexer(instream)
    token_stream = CommonTokenStream(lexer)
    parser = MiniDecafParser(token_stream)
    parser._errHandler = BailErrorStrategy()
    tree = parser.prog()
    visitor = MainVisitor()
    visitor.visit(tree)
    if args.out is not None:
        with open(args.out, 'w', encoding='utf8') as f:
            print('\n'.join(visitor.asm), file=f)
    else:
        print('\n'.join(visitor.asm))
