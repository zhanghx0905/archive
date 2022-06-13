#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h> // included for __init and __exit macros
#include <linux/io.h>
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/kthread.h>
#include <linux/module.h> // included for all kernel modules
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "debug_base.h"
#include "debug_control.h"
#include "debug_device.h"
#include "register.h"
#include "tools.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LC");
MODULE_DESCRIPTION("A instruction-granularity program instrument tool");

extern reg64_t context[100];

va_t bp = 0xaaaaaaaaa7b8;

extern struct base_addr_t* param;
extern struct task_struct** tsk;
extern struct file_operations file_ops;
msg_t msg;
extern char* msg_buffer;

static void pretest(void* addr)
{

    struct base_addr_t* param = (struct base_addr_t*)addr;

    printk(KERN_INFO "debug: test CoreSight");

    stop_cpu_via_cti(param->debug_register, param->cti_register);

    store_context(param->debug_register);

    restore_context(param->debug_register);

    restart_cpu_via_cti(param->debug_register, param->cti_register);

    printk(KERN_INFO "debug: CoreSight works");
}

/* ****************** */
/* Challenge: 2018011365 2018011363 */
/* ****************** */

static int listen(void* addr)
{
    reg32_t reg;
    reg64_t reg0, reg1; // reg0 for data register 0, reg1 for data register 1
    uint32_t location, set, way, offset, tot = 0;
    struct base_addr_t* param = (struct base_addr_t*)addr;
    printk(KERN_INFO "kthread1 start listening\n");

    while (!kthread_should_stop()) {
        reg = ioread32(param->debug_register + EDSCR_EL1);
        while ((reg & STATUS) == NON_DEBUG && !kthread_should_stop()) {
            reg = ioread32(param->debug_register + EDSCR_EL1);
        }
        if (!kthread_should_stop()) {

            store_context(param->debug_register);

            // DONE: get L1D cache tag and value

            // Switch to EL3: 0xf78f8003 <=> dcps3
            execute_ins_via_itr(param->debug_register, 0xd4a00003);

            for (set = 0; set < 128; ++set) {
                for (way = 0; way < 4; ++way) {
                    for (offset = 0; offset < 8; ++offset) {
                        location = (set << 6) | (way << 30) | (offset << 3);
                        msg.set = set;
                        msg.way = way;
                        msg.offset = offset;
                        // tag
                        // MSR S3_3_c15_c2_0, X0
                        write_register_via_x0(param->debug_register, 0xd51bf200, location);
                        // MRS X0, S3_3_c15_c0_0
                        reg0 = read_register_via_x0(param->debug_register, 0xd53bf000);
                        // MRS X0, S3_3_c15_c0_1
                        reg1 = read_register_via_x0(param->debug_register, 0xd53bf020);
                        msg.valid = ((reg1 >> 29) & 0b11) != 0;
                        msg.tag = ((reg1 & 0xfffffff) << 12) + (reg0 >> 31 << 11);
                        // data
                        // MSR S3_3_c15_c4_0, X0
                        write_register_via_x0(param->debug_register, 0xd51bf400, location);
                        // MRS X0, S3_3_c15_c0_0
                        msg.data0 = read_register_via_x0(param->debug_register, 0xd53bf000);
                        // MRS X0, S3_3_c15_c0_1
                        msg.data1 = read_register_via_x0(param->debug_register, 0xd53bf020);
                        // msg.data = (reg0 << 32) + reg1;

                        msg_encode(msg_buffer + (tot * 56 % MSG_BUFFER_LEN), &msg);
                        tot++;
                    }
                }
            }
            iowrite32(DBGBCR_DISABLE, param->debug_register + DBGBCRx_EL1(0));

            restore_context(param->debug_register);

            restart_cpu_via_cti(param->debug_register, param->cti_register);
        }
    }

    return 0;
}

static void enable(void* addr)
{
    struct base_addr_t* param = (struct base_addr_t*)addr;
    iowrite32(DBGBCR_ENABLE, param->debug_register + DBGBCRx_EL1(0));
}

static void reset(void* addr)
{
    struct base_addr_t* param = (struct base_addr_t*)addr;
    iowrite32(DBGBCR_DISABLE, param->debug_register + DBGBCRx_EL1(0));
    iowrite64(bp, param->debug_register + DBGBVRx_EL1(0));
}

static int __init challenge_init(void)
{
    printk(KERN_INFO "module init!\n");

    debugger_init();
    device_init();
    smp_call_function_single(2, pretest, param, 1);
    smp_call_function_single(2, reset, param, 1);

    tsk[0] = kthread_create(listen, param, "kthread%d", 1);
    kthread_bind(tsk[0], 2);
    wake_up_process(tsk[0]);

    smp_call_function_single(2, enable, param, 1);

    return 0;
}

static void __exit challenge_exit(void)
{
    kthread_stop(tsk[0]);

    smp_call_function_single(2, reset, param, 1);
    device_release();
    debugger_exit();

    printk(KERN_INFO "module exit!");
}
module_init(challenge_init);
module_exit(challenge_exit);