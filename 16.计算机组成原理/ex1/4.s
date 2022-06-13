.section .text
.globl _start
_start:
    li t0, 0x1 # a = t1 | t0
    li t1, 0x0
    li t2, 0x0 # b = t3 | t2
    li t3, 0x0
    li a0, 0x3c  # the (a0)th num
    la a1, 0x80400000
loop:  # t4 stores temporary variable
    # 高位运算
    mv  t4, t1
    add t1, t1, t3  # t1 = t3 + t1
    mv t3, t4  # t3 = t1
    # 低位运算
    mv t4, t0
    add t0, t0, t2  # t0 = t0 + t2
    bgeu t0, t4, normal
    add t1, t1, 1  # overflow
normal:
    mv t2, t4  # t2 = t0
    add a0, a0, -1
    bne a0, x0, loop
    # save
    sw t2, (a1)
    sw t3, 0x4(a1)
    jr ra
    