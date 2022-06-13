#ifndef BRANCH_ANALYSE_H
#define BRANCH_ANALYSE_H

#include <linux/io.h>
#include "register.h"

/* Types of AArch64 instructions. Instructions that are not branch types are recognized as NON_B type.*/
enum ins_type{
    NON_B, B_COND, CBNZ, CBZ, TBNZ, TBZ, B, BL, BLR, BR, RET, ERET
};

/* The cond filed in AArch64 ISA.*/
enum cond{
    EQ, NE, CS_HS, CC_LO, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL, NV
};

/* When given an instrucion, return its type.*/
int get_type(ins_t ins);

/* When given a branch instruction, the debugger can calculate its PRECISE target address through reading associated registers.*/
va_t get_target_addr(void __iomem *debug, ins_t ins, va_t ins_addr);

/* Return the cond encoded in CPU state through parsing the debug register DSPSR_EL0.*/
bool get_cond(void __iomem *debug, op_t cond_op);

#endif