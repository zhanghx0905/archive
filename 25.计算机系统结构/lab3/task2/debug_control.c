#include "debug_control.h"

uint64_t context[100];

reg64_t get_general_register(void __iomem* debug, op_t op_xt) {
    // store old x0
    uint64_t old_x0, xt; // old x0
    old_x0 = read_64bit_from_target(debug, 0xd5130400);
    // printk(KERN_INFO "op_xt = %d, ins will be 0x%lx", op_xt, 0xd5130400 | (op_xt & 0x1f));
    // 0xd51304xx <=> msr dbgdtr_el0, xt/xzr
    xt = read_64bit_from_target(debug, 0xd5130400 | (op_xt & 0x1f));
    // reload old x0
    send_64bit_to_target(debug, 0xd5330400, old_x0); //reload x0
    return xt;
}

reg64_t read_register_via_x0(void __iomem* debug, ins_t ins) {
    // ins: move the value from the system register to x0
    // store old x0
    uint64_t old_x0, data; // old x0
    old_x0 = read_64bit_from_target(debug, 0xd5130400);
    // read a system register to x0
    execute_ins_via_itr(debug, ins);
    data = read_64bit_from_target(debug, 0xd5130400);
    // reload old x0
    send_64bit_to_target(debug, 0xd5330400, old_x0); //reload x0
    return data; 
}

void write_register_via_x0(void __iomem* debug, ins_t ins, uint64_t val) {
    // ins: move the value from x0 to the system register
    // store old x0
    uint64_t old_x0; // old x0
    old_x0 = read_64bit_from_target(debug, 0xd5130400);
    // send the value to x0
    send_64bit_to_target(debug, 0xd5330400, val);
    // move the value from x0 to the system register
    execute_ins_via_itr(debug, ins);  
    // reload old x0
    send_64bit_to_target(debug, 0xd5330400, old_x0); //reload x0     
}

void store_context(void __iomem* debug) {
    int i;
    // 0xaa0003e0 <=> mov x0, x0; 0xd503201f <=> nop
    context[0] = read_64bit_from_target(debug, 0xd5130400);
    for (i = 1; i <= 30; ++ i)
        context[i] = get_general_register(debug, i);
    // 0xd53b4520 <=> mrs x0, dlr_el0
    context[98] = read_register_via_x0(debug, 0xd53b4520);
    // 0xd53b4500 <=> mrs x0, dspsr_el0
    context[99] = read_register_via_x0(debug, 0xd53b4500);   
}

void restore_context(void __iomem* debug) {
    int i;
    // 0xd51b4520 <=> msr dlr_el0, x0
    write_register_via_x0(debug, 0xd51b4520, context[98]);
    // 0xd51b4500 <=> msr dspsr_el0, x0
    write_register_via_x0(debug, 0xd51b4500, context[99]);
    for (i = 1; i <= 30; ++ i)
        send_64bit_to_target(debug, 0xd5330400 + i, context[i]);
    // 0xaa0003e0 <=> mov x0, x0; 0xd503201f <=> nop
    send_64bit_to_target(debug, 0xd5330400, context[0]);
}

void stop_cpu_via_cti(void __iomem* debug, void __iomem* cti) {
    uint32_t reg;
    // Step 1: Unlock debug and cross trigger reigsters
    iowrite32(0xc5acce55, debug + EDLAR_EL1);
    iowrite32(0x0, debug + OSLAR_EL1);
    iowrite32(0xc5acce55, cti + EDLAR_EL1);
    iowrite32(0x0, cti + OSLAR_EL1);
    // Step 2: Enable halting debug on the target processor
    reg = ioread32(debug + EDSCR_EL1);
    reg |= HDE;
    iowrite32(reg, debug + EDSCR_EL1);
    // Step 3: Send halt request to the target processor
    iowrite32(GLBEN, cti + CTICONTROL_EL1);
    reg = ioread32(cti + CTIGATE_EL1);
    reg &= ~GATE0;
    iowrite32(reg, cti + CTIGATE_EL1);
    reg = ioread32(cti + CTIOUTEN0_EL1);
    reg |= OUTEN0;
    iowrite32(reg, cti + CTIOUTEN0_EL1);
    reg = ioread32(cti + CTIAPPPULSE_EL1);
    reg |= APPPULSE0;
    iowrite32(reg, cti + CTIAPPPULSE_EL1);
    // Step 4: Wait the target processor to halt
    reg = ioread32(debug + EDSCR_EL1);
    while ((reg & STATUS) == NON_DEBUG)
        reg = ioread32(debug + EDSCR_EL1);
    reg = ioread32(cti + CTIINTACK_EL1);
    reg |= ACK0;
    iowrite32(reg, cti + CTIINTACK_EL1);
    reg = ioread32(cti + CTITRIGOUTSTATUS_EL1);
    while ((reg & TROUT0) == TROUT0)
        reg = ioread32(cti + CTITRIGOUTSTATUS_EL1);
}

void restart_cpu_via_cti(void __iomem* debug, void __iomem* cti) {
    uint32_t reg;
    // Step 1: Send restart request to the target processor
    reg = ioread32(cti + CTIGATE_EL1);
    reg &= ~GATE1;
    iowrite32(reg, cti + CTIGATE_EL1);
    reg = ioread32(cti + CTIOUTEN1_EL1);
    reg |= OUTEN1;
    iowrite32(reg, cti + CTIOUTEN1_EL1);
    reg = ioread32(cti + CTIAPPPULSE_EL1);
    reg |= APPPULSE1;
    iowrite32(reg, cti + CTIAPPPULSE_EL1);
    // Step 2: Wait the target processor to restart
    reg = ioread32(debug + EDSCR_EL1);
    while ((reg & STATUS) != NON_DEBUG)
        reg = ioread32(debug + EDSCR_EL1);
    reg = ioread32(cti + CTIINTACK_EL1);
    reg |= ACK1;
    iowrite32(reg, cti + CTIINTACK_EL1);
    reg = ioread32(cti + CTITRIGOUTSTATUS_EL1);
    while ((reg & TROUT1) == TROUT1)
        reg = ioread32(cti + CTITRIGOUTSTATUS_EL1);
}

reg64_t get_dlr(void) {
    return context[98];
}

reg64_t get_pc(void) {
    return get_dlr();
}

reg64_t get_dspsr(void) {
    return context[99];
}

reg64_t get_xt(int num) {
    if (num < 0 || num > 30) {
        printk("error: get wrong general register!");
        return -1;
    }
    return context[num];
}