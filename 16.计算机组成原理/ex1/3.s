.section .text
.globl _start
_start:
    li a0, 0x21
    li t2, 0x7F
loop:
# WRITE_SERIAL 函数，源于util.S
WRITE_SERIAL:                       # 写串口：将a0的低八位写入串口
    li t0, 0x10000000   # COM1 =  0x10000000
.TESTW:
    lb t1, %lo(5)(t0)  # 查看串口状态  COM_LSR_OFFSET = 5
    andi t1, t1, 0x20       # 截取写状态位 COM_LSR_THRE = 0x20
    bne t1, zero, .WSERIAL          # 状态位非零可写进入写
    j .TESTW                        # 检测验证，忙等待
.WSERIAL:
    sb a0, %lo(0)(t0)  # 写入寄存器a0中的值
    add a0, a0, 1
    bne a0, t2, loop
    jr ra
