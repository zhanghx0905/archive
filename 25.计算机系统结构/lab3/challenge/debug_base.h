#ifndef DEBUG_BASE_H
#define DEBUG_BASE_H

#include "register.h"
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/kthread.h>

#define KTHREAD_NUM 1

/* Some code comes from Nailgun Attack: https://github.com/ningzhenyu/nailgun */

/* Store the base address of debug registers and cti registers of target CPU. */
struct base_addr_t{
    void __iomem *debug_register;
    void __iomem *cti_register;
};

/* use ioremap to map a known pa to kernel va*/
void debugger_init(void);

/* use kfree to release kernel space*/
void debugger_exit(void);

/* Execute single instruction in debug states.
 * @debug: the base address of debug registers
 * @ins: AArch64 instructions (32 bits)
 */
void execute_ins_via_itr(void __iomem* debug, ins_t ins);

/* Debugger read a 64bit data from one of the general registers of the target CPU that is in debug state.
 * @debug: the base address of debug registers
 * @ins: the instruction that move data from Xt to dbgdtr_el0
 * @return: the 64bit data
 */
reg64_t read_64bit_from_target(void __iomem* debug, ins_t ins);

/* Debugger write a 64bit data to one of the general registers of the target CPU that is in debug state.
 * @debug: the base address of debug registers
 * @ins: the instruction that move date from dbgdtr_el0 to Xt
 * @val: the 64bit value
 */
void send_64bit_to_target(void __iomem* debug, ins_t ins, reg64_t val);

/* Debugger read a 32bit data from the virtual address va of the target CPU that is in debug state.
 * @debug: the base address of debug registers
 * @va: virtual address that you want to get a data from
 * @return: the 32bit data
 */
uint32_t read_memory_via_dcc(void __iomem* debug, va_t va);

#endif