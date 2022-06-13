# booth 算法的简单实现，为理解用
BIT = 5


def booth(a: int, b: int) -> int:
    def two_complement(num: int) -> int:
        return num if num >= 0 else num + 2**BIT

    def true_form(num: int) -> int:
        return num if num < 2**(BIT - 1) else num - 2**BIT
    a, b = two_complement(a), two_complement(b)
    res = a
    additional_bit = 0
    for _ in range(BIT):
        lowest_bit = res & 0x1
        cond = additional_bit - lowest_bit
        if cond == 1:
            res += (b << BIT)
        elif cond == -1:
            res -= (b << BIT)
        additional_bit = lowest_bit
        res >>= 1
    res = true_form(res % (2**BIT))
    return res


a, b = 3, -6
print(booth(a, b))
