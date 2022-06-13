#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/device.h> 

#include "register.h"
#include "debug_base.h"
#include "debug_control.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("LC");
MODULE_DESCRIPTION("A instruction-granularity program instrument tool");


extern reg64_t context[100];

// mov     x0, x26 这条指令的地址
va_t bp = 0xaaaaaaaaa788;

extern struct base_addr_t *param;
extern struct task_struct **tsk;


static void pretest(void *addr) {

    struct base_addr_t *param = (struct base_addr_t *)addr;

    printk(KERN_INFO "debug: test CoreSight");

    stop_cpu_via_cti(param->debug_register, param->cti_register);

    store_context(param->debug_register);

    restore_context(param->debug_register);

    restart_cpu_via_cti(param->debug_register, param->cti_register);

    printk(KERN_INFO "debug: CoreSight works");
}

static int listen(void *addr) {
    reg32_t reg;
    reg64_t reg1, reg2, reg3;

    struct base_addr_t *param = (struct base_addr_t *)addr;
    printk(KERN_INFO "kthread1 start listening\n");

    while (!kthread_should_stop()) {
        reg = ioread32(param->debug_register + EDSCR_EL1);
        while ((reg & STATUS) == NON_DEBUG && !kthread_should_stop()) {
            reg = ioread32(param->debug_register + EDSCR_EL1);
        }
        if(!kthread_should_stop()) {

            store_context(param->debug_register);

            reg1 = read_memory_via_dcc(param->debug_register, get_pc());
            // MSR DBGBCRx_EL1, x11
            reg2 = read_64bit_from_target(param->debug_register, 0xd513040b);
            // MSR DBGBCRx_EL1, x21
            reg3 = read_64bit_from_target(param->debug_register, 0xd5130415);

            printk(KERN_INFO "debug: reg1 = 0x%llx, reg2 = 0x%llx, reg3 = 0x%llx", reg1, reg2, reg3);

            iowrite32(DBGBCR_DISABLE, param->debug_register + DBGBCRx_EL1(0));

            restore_context(param->debug_register);
            // 0xaa1a03e0 <=> mov x0, x26
            if (reg1 == 0xaa1a03e0 && reg2 == 0x4869 && reg3 == 0x2333) {
                // MRS x26, DBGBCRx_EL1
                send_64bit_to_target(param->debug_register, 0xd533041a, 0x17082619);
            }

            restart_cpu_via_cti(param->debug_register, param->cti_register);
        }
    }

    return 0;
}

static void enable(void *addr) {
    struct base_addr_t *param = (struct base_addr_t *)addr;
    iowrite32(DBGBCR_ENABLE, param->debug_register + DBGBCRx_EL1(0));
}

static void reset(void *addr) {
    struct base_addr_t *param = (struct base_addr_t *)addr;
    iowrite32(DBGBCR_DISABLE, param->debug_register + DBGBCRx_EL1(0));
    iowrite64(bp, param->debug_register + DBGBVRx_EL1(0));
}

static int __init task2_init(void) {
    printk(KERN_INFO "module init!\n");

    debugger_init();

    // We use the Core 1 to read the SCR via debugging Core 0
    smp_call_function_single(2, pretest, param, 1);
    smp_call_function_single(2, reset, param, 1);

    tsk[0] = kthread_create(listen, param, "kthread%d", 1);
    kthread_bind(tsk[0], 2);
    wake_up_process(tsk[0]);

    smp_call_function_single(2, enable, param, 1);

    return 0;
}

static void __exit task2_exit(void) {
    kthread_stop(tsk[0]);

    smp_call_function_single(2, reset, param, 1);

    debugger_exit();

    printk(KERN_INFO "module exit!");
}
module_init(task2_init);
module_exit(task2_exit);