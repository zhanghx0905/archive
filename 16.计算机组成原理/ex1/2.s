.section .text
.globl _start
_start:
    li t0, 0x1 # t0, t1 = 1, 0
    li t1, 0x0
    li t3, 0xa
    la t4, 0x80400000
loop:
    mv  t2, t0
    add t0, t1, t0  # t0 = t0 + t1
    mv t1, t2  # t1 = t0
    sw t1, (t4)
    add t4, t4, 4
    add t3, t3, -1
    bne t3, x0, loop
    jr ra
    