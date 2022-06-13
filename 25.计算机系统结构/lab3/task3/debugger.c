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
#include "debug_device.h"
#include "debug_control.h"
#include "tools.h"
#include "branch_analyse.h"
#include "branch_predictor.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LC");
MODULE_DESCRIPTION("A instruction-granularity program instrument tool");

ulong _main_va = 0;
ulong _main_ret = 0;
ulong _init_va = 0;
ulong _fini_ret = 0;

module_param(_main_va, ulong, 0644);
module_param(_main_ret, ulong, 0644);
module_param(_init_va, ulong, 0644);
module_param(_fini_ret, ulong, 0644);

va_t main_va = 0;
va_t main_ret = 0;
va_t init_va = 0;
va_t fini_ret = 0;

extern reg64_t context[100];
extern char* msg_buffer;
extern struct file_operations file_ops;

int pre_times = 0;
int direction_correct_times = 0;
int address_correct_times = 0;

extern struct base_addr_t *param;
extern struct task_struct **tsk;

msg_t msg;

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
    va_t tar_addr, predict_addr;
    reg32_t reg;

    struct base_addr_t *param = (struct base_addr_t *)addr;
    printk(KERN_INFO "kthread1 start listening\n");

    while (!kthread_should_stop()) {
        reg = ioread32(param->debug_register + EDSCR_EL1);
        while ((reg & STATUS) == NON_DEBUG && !kthread_should_stop()) {
            reg = ioread32(param->debug_register + EDSCR_EL1);
        }
        if(!kthread_should_stop()) {

            store_context(param->debug_register);

            reg = read_memory_via_dcc(param->debug_register, get_pc());
            tar_addr = get_target_addr(param->debug_register, reg, get_pc());

            if (get_type(reg) == B_COND || get_type(reg) == CBZ || get_type(reg) == CBNZ || get_type(reg) == TBZ || get_type(reg) == TBNZ) {
                // printk(KERN_INFO "debug: pc = 0x%lx, target = 0x%lx", get_pc(), tar_addr);
                predict_addr = get_from_branch_predictor(get_pc());
                if (tar_addr == predict_addr) {
                    address_correct_times ++;
                    direction_correct_times ++;
                }
                else if (!((tar_addr != get_pc() + 4) ^ (predict_addr != get_pc() + 4))) {
                    direction_correct_times ++;
                }
                msg.pc = get_pc();
                msg.ins = reg;
                msg.tar = tar_addr; 
                msg.pre_tar = predict_addr;
                msg.dir = tar_addr != (get_pc() + 4);
                msg.pre_dir = predict_addr != (get_pc() + 4);
                msg_encode(msg_buffer + (pre_times * 48 % MSG_BUFFER_LEN) + sizeof(uint64_t), &msg);
                pre_times ++;
                branch_predictor_update(get_pc(), tar_addr);
            }

            if (get_pc() == main_ret) {
                ultostr(msg_buffer, pre_times);
                iowrite64(0x0, param->debug_register + DBGBVRx_EL1(0));
            }
            else if (tar_addr < init_va || tar_addr > fini_ret) {
                iowrite64(context[0x1e], param->debug_register + DBGBVRx_EL1(0));
            }
            else {
                iowrite64(tar_addr, param->debug_register + DBGBVRx_EL1(0));
            }

            mdelay(1);

            restore_context(param->debug_register);

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
    iowrite64(main_va, param->debug_register + DBGBVRx_EL1(0));
}

static int __init task3_init(void) {
    printk(KERN_INFO "module init!\n");

    debugger_init();

    device_init();

    branch_predictor_init();

    main_va = (va_t) _main_va;
    main_ret = (va_t) _main_ret;
    init_va = (va_t) _init_va;
    fini_ret = (va_t) _fini_ret;

    smp_call_function_single(2, pretest, param, 1);
    smp_call_function_single(2, reset, param, 1);

    tsk[0] = kthread_create(listen, param, "kthread%d", 1);
    kthread_bind(tsk[0], 2);
    wake_up_process(tsk[0]);

    smp_call_function_single(2, enable, param, 1);

    return 0;
}

static void __exit task3_exit(void) {
    kthread_stop(tsk[0]);

    smp_call_function_single(2, reset, param, 1);

    device_release();

    branch_predictor_release();

    debugger_exit();

    printk(KERN_INFO "pre_times = %d, direction_correct_times = %d, address_correct_times = %d", pre_times, direction_correct_times, address_correct_times);
    printk(KERN_INFO "module exit!");
}
module_init(task3_init);
module_exit(task3_exit);