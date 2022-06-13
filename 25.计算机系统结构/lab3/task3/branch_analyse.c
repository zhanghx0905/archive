#include "branch_analyse.h"

extern uint64_t get_pc(void);
extern uint64_t get_dspsr(void);
extern uint64_t get_xt(int num);
extern uint64_t signed_extern(uint64_t input, int highest_pos);

int get_type(ins_t ins) {
    int op31_24, op31_10, op4_0;
    op31_24 = (ins >> 24) & 0xff;
    op31_10 = (ins >> 10) & 0x3fffff;
    op4_0 = ins & 0x1f;
    if (op31_24 == 0b01010100)
        return B_COND;
    if ((op31_24 & 0x7f) == 0b0110101)
        return CBNZ;
    if ((op31_24 & 0x7f) == 0b0110100)
        return CBZ;
    if ((op31_24 & 0x7f) == 0b0110111)
        return TBNZ;
    if ((op31_24 & 0x7f) == 0b0110110)
        return TBZ;
    if ((op31_24 & 0xfc) == 0b00010100)
        return B;
    if ((op31_24 & 0xfc) == 0b10010100)
        return BL;
    if (op31_10 == 0b1101011000111111000000 && op4_0 == 0)
        return BLR;
    if (op31_10 == 0b1101011000011111000000 && op4_0 == 0)
        return BR;
    if (op31_10 == 0b1101011001011111000000 && op4_0 == 0)
        return RET;
    if (ins == 0b11010110100111110000001111100000)
        return ERET;
    return NON_B;
}

bool get_cond(void __iomem* debug, op_t cond_op) {
    reg32_t reg;
    int n,z,c,v;
    reg = get_dspsr();  // dspsr_el0, contains nzcv

    n = (reg >> 31) & 1;
    z = (reg >> 30) & 1;
    c = (reg >> 29) & 1;
    v = (reg >> 28) & 1;
    switch(cond_op){
        case EQ:
            return z;
        case NE:
            return !z;
        case CS_HS:
            return c;
        case CC_LO:
            return !c;
        case MI:
            return n;
        case PL:
            return !n;
        case VS:
            return v;
        case VC:
            return !v;
        case HI:
            return c && !z;
        case LS:
            return !(c && !z);
        case GE:
            return n == v;
        case LT:
            return n != v;
        case GT:
            return (!z) && (n == v);
        case LE:
            return !((!z) && (n == v));
        case AL:
        case NV:
        default:
            return true;
    }
}

va_t get_target_addr(void __iomem* debug, ins_t ins, va_t ins_addr) {
    int offset;  // offset from pc
    va_t pc = ins_addr;  // current pc
    bool cond;  // czsv
    reg64_t xt;  // a general purpose register
    int bit_pos;  // use in TBZ and TBNZ
    // not branch
    if (get_type(ins) == NON_B)
        return pc + 4;
    // branch
    switch(get_type(ins)){
        case B_COND:
            cond = get_cond(debug, (uint8_t)ins & 0xf);  // if cond then jump
            offset = signed_extern(((ins >> 5) & 0x7ffff) << 2, 20);
            if(cond)
                return pc + offset;
            else
                return pc + 4;
        case CBNZ:
        case CBZ:
            xt = get_xt(ins & 0x1f);
            offset = signed_extern(((ins >> 5) & 0x7ffff) << 2, 20);
            if (((ins >> 31) & 1) == 0) { // sf = 0, use 32 bit
                cond = ((ins >> 24) & 1) ^ ((xt & 0xffffffff) == 0);  // if cond then jump
                if(cond)
                    return pc + offset;
                else
                    return pc + 4;
            }
            else {  // sf = 1, use 64 bit
                cond = ((ins >> 24) & 1) ^ (xt == 0);  // if cond then jump
                if(cond)
                    return pc + offset;
                else
                    return pc + 4;                   
            }
        case TBNZ:
        case TBZ:
            xt = get_xt(ins & 0x1f);
            offset = signed_extern(((ins >> 5) & 0x3fff) << 2, 15);
            bit_pos = ((ins >> 25) & 0x20) | ((ins >> 19) & 0x1f);
            cond = ((ins >> 24) & 1) ^ (((xt >> bit_pos) & 1) == 0);
            if(cond)
                return pc + offset;
            else
                return pc + 4;
        case B:
        case BL:
            offset = signed_extern((ins & 0x3ffffff) << 2, 27);
            return pc + offset;
        case BR:
        case BLR:
        case RET:
            xt = get_xt((ins >> 5) & 0x1f);
            return xt;
        case ERET:

        default:
            return pc + 4;
    }
    return 0;
}