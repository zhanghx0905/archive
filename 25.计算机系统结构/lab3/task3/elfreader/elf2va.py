from capstone import *
from capstone.arm64 import *
import elf2struct

branch_list = [ARM64_INS_B, ARM64_INS_BL, ARM64_INS_BLR, ARM64_INS_BR, ARM64_INS_CBNZ, ARM64_INS_CBZ, ARM64_INS_RET, ARM64_INS_TBNZ, ARM64_INS_TBZ]

def display_ins_info(insn):
    print("0x%x:\t%s\t%s" %(insn.address, insn.mnemonic, insn.op_str))
    if len(insn.operands) > 0:
        print("\tNumber of operands: %u" %len(insn.operands))
        c = -1
        for i in insn.operands:
            c += 1
            if i.type == ARM64_OP_REG:
                print("\t\toperands[%u].type: REG = %s" %(c, insn.reg_name(i.value.reg)))
            if i.type == ARM64_OP_IMM:
                print("\t\toperands[%u].type: IMM = 0x%x" %(c, i.value.imm))
            if i.type == ARM64_OP_CIMM:
                print("\t\toperands[%u].type: C-IMM = %u" %(c, i.value.imm))
            if i.type == ARM64_OP_FP:
                print("\t\toperands[%u].type: FP = %f" %(c, i.value.fp))
            if i.type == ARM64_OP_MEM:
                print("\t\toperands[%u].type: MEM" %c)
                if i.value.mem.base != 0:
                    print("\t\t\toperands[%u].mem.base: REG = %s" \
                        %(c, insn.reg_name(i.value.mem.base)))
                if i.value.mem.index != 0:
                    print("\t\t\toperands[%u].mem.index: REG = %s" \
                        %(c, insn.reg_name(i.value.mem.index)))
                if i.value.mem.disp != 0:
                    print("\t\t\toperands[%u].mem.disp: 0x%x" \
                        %(c, i.value.mem.disp))

            if i.shift.type != ARM64_SFT_INVALID and i.shift.value:
                print("\t\t\tShift: type = %u, value = %u" \
                    %(i.shift.type, i.shift.value))

            if i.ext != ARM64_EXT_INVALID:
                print("\t\t\tExt: %u" %i.ext)

    if insn.writeback:
        print("\tWrite-back: True")
    if not insn.cc in [ARM64_CC_AL, ARM64_CC_INVALID]:
        print("\tCode condition: %u" %insn.cc)
    if insn.update_flags:
        print("\tUpdate-flags: True")

def get_code_from_elf(filename):
    codes = []
    name_list = [".init", ".plt", ".text", ".fini"]
    temp = 0
    code_sections = elf2struct.get_offva_by_name_in_sections(filename, name_list)
    for key in code_sections: 
        code_sections[key]["code"] = elf2struct.get_code(filename, code_sections[key]["va"], code_sections[key]["size"])
    return code_sections

'''
ARM64_INS_B = 16; ARM64_INS_BL = 21; ARM64_INS_BLR = 22; ARM64_INS_BR = 23; ARM64_INS_RET = 209
ARM64_INS_CBNZ = 26; ARM64_INS_CBZ = 27; ARM64_INS_TBNZ = 346; ARM64_INS_TBZ = 348
'''
def branch_ins_filter(ins):
    global branch_list
    if ins.id in branch_list:
        # print("0x%x:\t%s\t%s" %(ins.address, ins.mnemonic, ins.op_str))
        return True
    else:
        return False

def get_branch_ins_addr(filename, start_va):
    md = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
    md.detail = True
    addr_list = []
    code_sections = get_code_from_elf(filename)

    for key in code_sections:
        for ins in md.disasm(code_sections[key]["code"], code_sections[key]["va"]):
            # display_ins_info(insn)
            if branch_ins_filter(ins):
                addr_list.append(ins.address + start_va)

    return addr_list

def example():
    filename = "../testing/helper_static"
    start_va = 0x400000
    addr_list = get_branch_ins_addr(filename, start_va)
    for i in range(0, len(addr_list)):
        print("0x%x" % addr_list[i], end = ' ')
    print()

example()