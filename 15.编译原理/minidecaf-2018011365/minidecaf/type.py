from enum import Enum
from io import UnsupportedOperation
from typing import List


ValueCat = Enum('ValueCat', 'LVALUE RVALUE')


class MiniType:
    def __init__(self, name, vcat=ValueCat.RVALUE) -> None:
        self.name = name
        self.vcat = vcat

    def __str__(self) -> str:
        return f'{self.name}({self.vcat})'

    def __eq__(self, o: object) -> bool:
        return isinstance(o, self.__class__)

    def referenced(self):
        raise UnsupportedOperation(f"{self.name} can't be referenced.")

    def dereferenced(self):
        raise UnsupportedOperation(f"{self.name} can't be referenced.")

    def value_cast(self, target_cat):
        return self


class NoType(MiniType):
    def __init__(self) -> None:
        super().__init__('NoType')

    def __len__(self):
        raise UnsupportedOperation("NoType doesn't have length.")


class PointerType(MiniType):
    def __init__(self, star_cnt, vcat=ValueCat.RVALUE) -> None:
        super().__init__(f'PointerType<{star_cnt}>', vcat)
        self.star_cnt = star_cnt

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


class IntType(MiniType):
    def __init__(self, vcat=ValueCat.RVALUE) -> None:
        super().__init__('IntType', vcat)

    def __len__(self):
        return 4

    def __eq__(self, o: object) -> bool:
        return isinstance(o, IntType) and o.vcat is self.vcat

    def referenced(self):
        if self.vcat == ValueCat.LVALUE:
            return PointerType(1)
        raise UnsupportedOperation("Rvalue can't be referenced.")

    def value_cast(self, target_cat):
        return IntType(target_cat)


class ArrayType(MiniType):
    def __init__(self, base: MiniType, length: int):
        super().__init__(f'ArrayType<{length}>')
        self.base = base
        self.size = length * len(base)

    def __len__(self):
        return self.size

    def __eq__(self, o: object) -> bool:
        return isinstance(o, ArrayType) and self.base == o.base and len(self) == len(o)

    def value_cast(self, target_cat):
        if target_cat == ValueCat.LVALUE:
            raise UnsupportedOperation("Array must be RVALUE.")
        return self


class FunType:
    def __init__(self,
                 ret_type: MiniType,
                 para_types: List[MiniType]) -> None:
        self.ret_type = ret_type
        self.para_types = tuple(para_types)

    def __eq__(self, o: object) -> bool:
        if self.ret_type != o.ret_type or self.para_types != o.para_types:
            return False
        return True
