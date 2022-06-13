from collections import deque, namedtuple, UserList
from functools import partial
from itertools import count
from typing import Type

from antlr4 import ParserRuleContext

from .generated.MiniDecafParser import MiniDecafParser
from .generated.MiniDecafVisitor import MiniDecafVisitor
from .type import (ArrayType, FunType, IntType, MiniType, NoType, PointerType,
                   ValueCat)
from .utils import BIN_TO_ASM, INT_MAX, INT_MIN, UNA_TO_ASM

# 全局变量  name : MiniType
declared_global_table = {}
inited_global_table = {}

# 局部变量
Symbol = namedtuple('Symbol', 'offset ty')
symbol_table = []  # 局部变量栈 [ {name : Symbol}, ...]

# 函数  name : FunType
declared_fun_table = {}
defined_fun_table = {}


def raise_error(ctx: ParserRuleContext):
    raise RuntimeError(f'Error({ctx.start.line}, {ctx.start.column})\n')


def look_up_symbol(name: str):
    for table in reversed(symbol_table):  # 从内向外寻找
        if name in table:
            return table[name]
    return None


class AsmList(UserList):
    def write(self, *inputs):
        self.extend(inputs)

    def push(self, *regs):
        for reg in regs:
            self.write('\taddi sp, sp, -4',
                       f'\tsw {reg}, 0(sp)')

    def pop(self, *regs):
        for reg in regs:
            self.write(f'\tlw {reg}, 0(sp)',
                       '\taddi sp, sp, 4')


class MainVisitor(MiniDecafVisitor):
    def __init__(self) -> None:
        self.asm = AsmList()
        self.write = self.asm.write
        self.push = self.asm.push
        self.pop = self.asm.pop

        self.rcast = partial(self.type_check, expected_ty=MiniType, vcat=ValueCat.RVALUE)
        self.get_loop_nid = partial(next, count())
        self.get_cond_nid = partial(next, count())

        self.contains_main = False
        self.cur_func = None
        self.local_var_cnt = 0

        self.loops_stack = []

    def do_binary_op(self, op, handler=None):
        self.pop('t1', 't0')
        if handler:
            handler()
        self.write(BIN_TO_ASM[op])
        self.push('t0')

    def type_check(self, actual_ty: MiniType,
                   ctx: ParserRuleContext,
                   expected_ty: Type = MiniType,
                   vcat=ValueCat.RVALUE) -> MiniType:
        if not issubclass(actual_ty.__class__, expected_ty):
            raise_error(ctx)
        if vcat == ValueCat.LVALUE and actual_ty.vcat == ValueCat.RVALUE:
            raise_error(ctx)
        if vcat == ValueCat.RVALUE and actual_ty.vcat == ValueCat.LVALUE:
            self.pop('t0')
            self.write('\tlw t0, 0(t0)')
            self.push('t0')
        return actual_ty.value_cast(vcat)

    def visitProg(self, ctx: MiniDecafParser.ProgContext):
        for fun in ctx.children:
            self.visit(fun)

        if not self.contains_main:
            raise_error(ctx)

        # 未初始化的全局变量
        for name, ty in declared_global_table.items():
            if name not in inited_global_table:
                self.write(f'\t.comm {name}, {len(ty)}, 4')
        return NoType()

    def visitDeclaredFunc(self, ctx: MiniDecafParser.DeclaredFuncContext):
        name = ctx.IDENT(0).getText()
        if name in declared_global_table:
            raise_error(ctx)

        ret_type = self.visit(ctx.ty(0))
        para_types = []
        for i in range(1, len(ctx.ty())):
            para_types.append(self.visit(ctx.ty(i)))

        fun_type = FunType(ret_type, para_types)
        if name in declared_fun_table and declared_fun_table[name] != fun_type:
            raise_error(ctx)
        declared_fun_table[name] = fun_type
        return NoType()

    def visitDefinedFunc(self, ctx: MiniDecafParser.DefinedFuncContext):
        name = ctx.IDENT(0).getText()
        if name in declared_global_table:
            raise_error(ctx)

        self.cur_func = name
        if name == 'main':
            self.contains_main = True

        self.write("\t.text",
                   f"\t.global {name}",
                   f"{name}:")

        if name in defined_fun_table:
            raise_error(ctx)

        ret_type = self.visit(ctx.ty(0))
        para_types = []
        for i in range(1, len(ctx.ty())):
            para_types.append(self.visit(ctx.ty(i)))
        fun_type = FunType(ret_type, para_types)

        if name in declared_fun_table and declared_fun_table[name] != fun_type:
            raise_error(ctx)

        declared_fun_table[name] = fun_type
        defined_fun_table[name] = fun_type

        self.push('ra', 'fp')
        self.write('\tmv fp, sp')
        backtrace_pos = len(self.asm)
        self.local_var_cnt = 0

        # 开辟作用域
        symbol_table.append({})
        # 将参数加入作用域
        for idx in range(1, len(ctx.IDENT())):
            para_name = ctx.IDENT(idx).getText()
            if para_name in symbol_table[-1]:
                raise_error(ctx)
            if idx < 9:
                self.local_var_cnt += 1
                self.write(f'\tsw a{idx-1}, {-4*idx}(fp)')
                symbol = Symbol(-4 * idx, fun_type.para_types[idx - 1].value_cast(ValueCat.LVALUE))
            else:
                symbol = Symbol(4 * (idx - 9 + 2),
                                fun_type.para_types[idx - 1].value_cast(ValueCat.LVALUE))
            symbol_table[-1][para_name] = symbol
        for item in ctx.blockItem():
            self.visit(item)
        symbol_table.pop()

        self.asm.insert(backtrace_pos, f'\taddi sp, sp, {-4*self.local_var_cnt}')

        self.write(f'.exit.{name}:',
                   '\tlw a0, 0(sp)',
                   '\tmv sp, fp')
        self.pop('fp', 'ra')
        self.write('\tret')

        return NoType()

    def visitGlobalIntOrPointerDecl(self, ctx: MiniDecafParser.GlobalIntOrPointerDeclContext):
        ''' 全局变量可以多次声明，但只能被初始化一次 '''
        name = ctx.IDENT().getText()
        if name in declared_fun_table:
            raise_error(ctx)
        ty = self.visit(ctx.ty())
        if name in declared_global_table and declared_global_table[name] != ty:
            raise_error(ctx)

        declared_global_table[name] = ty.value_cast(ValueCat.LVALUE)
        if (num := ctx.NUM()) is not None:
            if name in inited_global_table:
                raise_error(ctx)
            inited_global_table[name] = ty.value_cast(ValueCat.RVALUE)

            self.write('\t.data',
                       '\t.align 4',
                       f'{name}:',
                       f'\t.word {num.getText()}')
        return NoType()

    def visitGlobalArrayDecl(self, ctx: MiniDecafParser.GlobalArrayDeclContext):
        name = ctx.IDENT().getText()
        if name in declared_fun_table:
            raise_error(ctx)

        types = deque()
        types.append(self.visit(ctx.ty()).value_cast(ValueCat.LVALUE))
        for num in reversed(ctx.NUM()):
            i = int(num.getText())
            if i > INT_MAX or i < INT_MIN:
                raise_error(ctx)
            if i == 0:
                raise_error(ctx)
            types.appendleft(ArrayType(types[0], i))
        ty = types[0]

        if name in declared_global_table and declared_global_table[name] != ty:
            raise_error(ctx)
        declared_global_table[name] = ty

        return NoType()

    def visitLocalArrayDecl(self, ctx: MiniDecafParser.LocalArrayDeclContext):
        name = ctx.IDENT().getText()
        if name in symbol_table[-1]:
            raise_error(ctx)
        types = deque()
        types.append(self.visit(ctx.ty()).value_cast(ValueCat.LVALUE))
        for num in reversed(ctx.NUM()):
            i = int(num.getText())
            if i > INT_MAX or i < INT_MIN:
                raise_error(ctx)
            if i == 0:
                raise_error(ctx)
            types.appendleft(ArrayType(types[0], i))
        ty = types[0]
        self.local_var_cnt += len(ty) // 4
        symbol_table[-1][name] = Symbol(-4 * self.local_var_cnt, ty)

        return NoType()

    def visitLocalIntOrPointerDecl(self, ctx: MiniDecafParser.LocalIntOrPointerDeclContext):
        name = ctx.IDENT().getText()
        if name in symbol_table[-1]:
            raise_error(ctx)
        ty = self.visit(ctx.ty())
        self.local_var_cnt += 1
        symbol_table[-1][name] = Symbol(-4 * self.local_var_cnt,
                                        ty.value_cast(ValueCat.LVALUE))
        expr = ctx.expr()
        if expr is not None:
            expr_ty = self.rcast(self.visit(expr), ctx)
            if expr_ty != ty:
                raise_error(ctx)
            self.pop('t0')
            self.write(f'\tsw t0, {-4*self.local_var_cnt}(fp)')
        return NoType()

    def visitReturnStmt(self, ctx: MiniDecafParser.ReturnStmtContext):
        ret_ty = self.rcast(self.visit(ctx.expr()), ctx)
        excepted_ty = defined_fun_table[self.cur_func].ret_type
        if ret_ty != excepted_ty:
            raise_error(ctx)
        self.write(f'\tj .exit.{self.cur_func}')
        return NoType()

    def visitExprStmt(self, ctx: MiniDecafParser.ExprStmtContext):
        expr = ctx.expr()
        if expr is not None:
            self.visit(expr)
            self.pop('t0')
        return NoType()

    def visitIfStmt(self, ctx: MiniDecafParser.IfStmtContext):
        cur_No = self.get_cond_nid()

        self.type_check(self.visit(ctx.expr()), ctx, IntType)
        self.pop('t0')
        self.write(f'\tbeqz t0, .else{cur_No}')
        self.visit(ctx.stmt(0))
        self.write(f'\tj .afterCondNo{cur_No}',
                   f'.else{cur_No}:')
        if len(ctx.stmt()) > 1:
            self.visit(ctx.stmt(1))
        self.write(f'.afterCondNo{cur_No}:')
        return NoType()

    def visitBlockStmt(self, ctx: MiniDecafParser.BlockStmtContext):
        symbol_table.append({})
        for item in ctx.blockItem():
            self.visit(item)
        symbol_table.pop()
        return NoType()

    def visitWhileStmt(self, ctx: MiniDecafParser.WhileStmtContext):
        cur_No = self.get_loop_nid()
        self.write(f'.beforeLoop{cur_No}:',
                   f'.continueLoop{cur_No}:')
        self.type_check(self.visit(ctx.expr()), ctx, IntType)
        self.pop('t0')
        self.write(f'\tbeqz t0, .afterLoop{cur_No}')

        self.loops_stack.append(cur_No)
        self.visit(ctx.stmt())
        self.loops_stack.pop()

        self.write(f'\tj .beforeLoop{cur_No}',
                   f'.afterLoop{cur_No}:')
        return NoType()

    def visitForStmt(self, ctx: MiniDecafParser.ForStmtContext):
        cur_No = self.get_loop_nid()
        init_expr, cond_expr, after_expr = None, None, None

        for i in range(ctx.getChildCount()):
            if isinstance(ctx.getChild(i), MiniDecafParser.ExprContext):
                expr = ctx.getChild(i)
                if ctx.getChild(i - 1).getText() == "(":
                    init_expr = expr
                elif ctx.getChild(i + 1).getText() == ";":
                    cond_expr = expr
                else:
                    after_expr = expr
        symbol_table.append({})
        if init_expr is not None:
            self.visit(init_expr)
            self.write('\taddi sp, sp, 4')
        if ctx.localDel() is not None:
            self.visit(ctx.localDel())

        self.write(f'.beforeLoop{cur_No}:')
        if cond_expr is not None:
            self.type_check(self.visit(cond_expr), ctx, IntType)
            self.write('\tlw t1, 0(sp)',
                       '\taddi sp, sp, 4',
                       f'\tbeqz t1, .afterLoop{cur_No}')

        self.loops_stack.append(cur_No)
        self.visit(ctx.stmt())
        self.loops_stack.pop()

        self.write(f'.continueLoop{cur_No}:')
        if after_expr is not None:
            self.visit(after_expr)
            self.write('\taddi sp, sp, 4')
        symbol_table.pop()

        self.write(f'\tj .beforeLoop{cur_No}',
                   f'.afterLoop{cur_No}:')
        return NoType()

    def visitDoStmt(self, ctx: MiniDecafParser.DoStmtContext):
        cur_No = self.get_loop_nid()
        self.write(f'.beforeLoop{cur_No}:')

        self.loops_stack.append(cur_No)
        self.visit(ctx.stmt())
        self.loops_stack.pop()

        self.write(f'.continueLoop{cur_No}:')
        self.type_check(self.visit(ctx.expr()), ctx, IntType)
        self.pop('t0')
        self.write(f'\tbnez t0, .beforeLoop{cur_No}',
                   f'.afterLoop{cur_No}:')

        return NoType()

    def visitBreakStmt(self, ctx: MiniDecafParser.BreakStmtContext):
        if len(self.loops_stack) == 0:
            raise_error(ctx)
        self.write(f'\tj .afterLoop{self.loops_stack[-1]}')
        return NoType()

    def visitContinueStmt(self, ctx: MiniDecafParser.ContinueStmtContext):
        if len(self.loops_stack) == 0:
            raise_error(ctx)
        self.write(f'\tj .continueLoop{self.loops_stack[-1]}')
        return NoType()

    def visitExpr(self, ctx: MiniDecafParser.ExprContext):
        if len(ctx.children) > 1:
            unary_ty = self.type_check(self.visit(
                ctx.unary()), ctx, vcat=ValueCat.LVALUE)
            expr_ty = self.rcast(self.visit(ctx.expr()), ctx)
            if unary_ty.value_cast(ValueCat.RVALUE) != expr_ty:
                raise_error(ctx)
            self.pop('t1', 't0')
            self.write('\tsw t1, 0(t0)')
            self.push('t0')
            return unary_ty
        return self.visit(ctx.ternary())

    def visitTernary(self, ctx: MiniDecafParser.TernaryContext):
        if len(ctx.children) > 1:
            cur_No = self.get_cond_nid()
            self.type_check(self.visit(ctx.lor()), ctx, IntType)

            self.pop('t0')
            self.write(f'\tbeqz t0, .else{cur_No}')
            if_ty = self.rcast(self.visit(ctx.expr()), ctx)
            self.write(f'\tj .afterCondNo{cur_No}',
                       f'.else{cur_No}:')
            else_ty = self.rcast(self.visit(ctx.ternary()), ctx)
            self.write(f'.afterCondNo{cur_No}:')
            if if_ty != else_ty:
                raise_error(ctx)
            return if_ty
        return self.visit(ctx.lor())

    def visitLor(self, ctx: MiniDecafParser.LorContext):
        if len(ctx.children) > 1:
            self.type_check(self.visit(ctx.lor(0)), ctx, IntType)
            self.type_check(self.visit(ctx.lor(1)), ctx, IntType)
            self.do_binary_op(ctx.getChild(1).getText())
            return IntType()
        return self.visit(ctx.land())

    def visitLand(self, ctx: MiniDecafParser.LandContext):
        if len(ctx.children) > 1:
            self.type_check(self.visit(ctx.land(0)), ctx, IntType)
            self.type_check(self.visit(ctx.land(1)), ctx, IntType)
            self.do_binary_op(ctx.getChild(1).getText())
            return IntType()
        return self.visit(ctx.leq())

    def visitLeq(self, ctx: MiniDecafParser.LeqContext):
        if len(ctx.children) > 1:
            left_ty = self.rcast(self.visit(ctx.leq(0)), ctx)
            right_ty = self.rcast(self.visit(ctx.leq(1)), ctx)
            if left_ty != right_ty:
                raise_error(ctx)
            if isinstance(left_ty, ArrayType) and isinstance(right_ty, ArrayType):
                raise_error(ctx)
            self.do_binary_op(ctx.getChild(1).getText())
            return IntType()
        return self.visit(ctx.rel())

    def visitRel(self, ctx: MiniDecafParser.RelContext):
        if len(ctx.children) > 1:
            self.type_check(self.visit(ctx.rel(0)), ctx, IntType)
            self.type_check(self.visit(ctx.rel(1)), ctx, IntType)
            self.do_binary_op(ctx.getChild(1).getText())
            return IntType()
        return self.visit(ctx.add())

    def visitAdd(self, ctx: MiniDecafParser.AddContext):
        if len(ctx.children) > 1:
            left_ty = self.rcast(self.visit(ctx.add(0)), ctx)
            right_ty = self.rcast(self.visit(ctx.add(1)), ctx)
            ret_ty = NoType()
            op = ctx.getChild(1).getText()
            if isinstance(left_ty, IntType) and isinstance(right_ty, IntType):
                op += 'i'   # int相加
                ret_ty = IntType()
            elif isinstance(left_ty, PointerType) and isinstance(right_ty, IntType):
                op += 'pleft'   # 指针+-int
                ret_ty = left_ty
            elif isinstance(left_ty, IntType) and isinstance(right_ty, PointerType):
                if op == '-':
                    raise_error(ctx)
                op += 'pright'  # int+指针
                ret_ty = right_ty
            elif isinstance(left_ty, PointerType) and right_ty == left_ty:
                if op == '+':
                    raise_error(ctx)
                op += 'pp'  # 指针-指针
                ret_ty = IntType()
            else:
                raise_error(ctx)
            self.do_binary_op(op)
            return ret_ty
        return self.visit(ctx.mul())

    def visitMul(self, ctx: MiniDecafParser.MulContext):
        if len(ctx.children) > 1:
            self.type_check(self.visit(ctx.mul(0)), ctx, IntType)
            self.type_check(self.visit(ctx.mul(1)), ctx, IntType)
            self.do_binary_op(ctx.getChild(1).getText())
            return IntType()
        return self.visit(ctx.unary())

    def visitOpUnary(self, ctx: MiniDecafParser.OpUnaryContext):
        ty = self.visit(ctx.unary())
        op = ctx.getChild(0).getText()

        if op == '*':
            return self.rcast(ty, ctx).dereferenced()
        elif op == '&':
            return ty.referenced()
        else:
            self.type_check(ty, ctx, IntType)
            self.pop('t0')
            self.write(f'{UNA_TO_ASM[op]} t0, t0')
            self.push('t0')
            return IntType()

    def visitCastUnary(self, ctx: MiniDecafParser.CastUnaryContext):
        src_ty = self.visit(ctx.unary())
        dst_ty = self.visit(ctx.ty())
        return dst_ty.value_cast(src_ty.vcat)

    def visitPostfixUnary(self, ctx: MiniDecafParser.PostfixUnaryContext):
        return self.visit(ctx.postfix())

    def visitCallPostfix(self, ctx: MiniDecafParser.CallPostfixContext):
        name = ctx.IDENT().getText()
        if name not in declared_fun_table:
            raise_error(ctx)
        fun_type = declared_fun_table[name]
        if len(fun_type.para_types) != len(ctx.expr()):
            raise_error(ctx)

        for idx in range(len(ctx.expr()) - 1, -1, -1):
            ty = self.rcast(self.visit(ctx.expr(idx)), ctx)
            if ty != fun_type.para_types[idx]:
                raise_error(ctx)
            if idx < 8:
                self.pop(f'a{idx}')

        self.write(f'\tcall {name}')
        self.push('a0')
        return fun_type.ret_type

    def visitSubscriptPostfix(self, ctx: MiniDecafParser.SubscriptPostfixContext):
        postfix_ty = self.rcast(self.visit(ctx.postfix()), ctx)
        self.type_check(self.visit(ctx.expr()), ctx, IntType)
        op = '[]'
        ret_ty, handler = None, None
        if isinstance(postfix_ty, PointerType):
            op += 'p'
            ret_ty = postfix_ty.dereferenced()
        elif isinstance(postfix_ty, ArrayType):
            op += 'a'
            ret_ty = postfix_ty.base
            handler = partial(self.write, f'\tli t2, {len(ret_ty)}')
        else:
            raise_error(ctx)
        self.do_binary_op(op, handler)
        return ret_ty

    def visitPrimaryPostfix(self, ctx: MiniDecafParser.PrimaryPostfixContext):
        return self.visit(ctx.primary())

    def visitNumPrimary(self, ctx: MiniDecafParser.NumPrimaryContext):
        val = int(ctx.NUM().getText())
        if val < INT_MIN or val > INT_MAX:
            raise_error(ctx)

        self.write(f'\tli t0, {val}')
        self.push('t0')
        return IntType()

    def visitParenthesizedPrimary(self, ctx: MiniDecafParser.ParenthesizedPrimaryContext):
        return self.visit(ctx.expr())

    def visitIdentPrimary(self, ctx: MiniDecafParser.IdentPrimaryContext):
        name = ctx.IDENT().getText()
        if (symbol := look_up_symbol(name)) is not None:
            self.write(f'\taddi t0, fp, {symbol.offset}')
            self.push('t0')
            return symbol.ty
        elif name in declared_global_table:             # 读取全局变量
            self.write(f'\tlui t0, %hi({name})',
                       f'\taddi t0, t0, %lo({name})')
            self.push('t0')
            return declared_global_table[name]
        else:
            raise_error(ctx)

    def visitTy(self, ctx: MiniDecafParser.TyContext):
        star_cnt = ctx.getChildCount() - 1
        if star_cnt == 0:
            return IntType()
        return PointerType(star_cnt)
