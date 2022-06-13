#ifndef DEBUG_CONTROL_H
#define DEBUG_CONTROL_H

#include <linux/io.h>
#include "debug_base.h"

/* Some code comes from Nailgun Attack: https://github.com/ningzhenyu/nailgun */

/* Get general register value from target CPU.
 * @debug: the base address of debug registers
 * @op_xt: choose the register from 0 to 30
 * @return: the 64bit data in a general register
 */
reg64_t get_general_register(void __iomem* debug, op_t op_xt);

/* Get system register value from target CPU.
 * @debug: the base address of debug registers
 * @ins: the MRS instrustion that move a system register to a general register
 * @return: the 64bit data in a system register
 */
reg64_t read_register_via_x0(void __iomem* debug, ins_t ins);

/* Write system register value to target CPU.
 * @debug: the base address of debug registers
 * @ins: the MSR instrustion that move a general register to a system register
 * @val: the 64bit data
 */
void write_register_via_x0(void __iomem* debug, ins_t ins, uint64_t val);

/* Store general registers and some system registers of target CPU when entering debug state.
 * debug: the base address of debug registers
 */
void store_context(void __iomem* debug);

/* Restore general registers and some system registers of target CPU when exiting debug state.
 * @debug: the base address of debug registers
 */
void restore_context(void __iomem* debug);

/* Store target CPU and make its state from normal state to debug state.
 * @debug: the base address of debug registers
 * @cti: the base address of cti registers
 */
void stop_cpu_via_cti(void __iomem* debug, void __iomem* cti);

/* Restart target CPU and make its state from debug state to normal state.
 * @debug: the base address of debug registers
 * @cti: the base address of cti registers
 */
void restart_cpu_via_cti(void __iomem* debug, void __iomem* cti);

/* Get the value of DLR_EL0
 * @return: the 64bit value
 */
reg64_t get_dlr(void);

/* Get the value of last pc before entering debug state.
 * @return: the 64bit value
 */
reg64_t get_pc(void);

/* Get the value of DSPSR_EL0
 * @return: the 64bit value
 */
reg64_t get_dspsr(void);

/* Get the value of a general register Xt
 * @num: the index of X[t]
 * @return: the 64bit value
 */
reg64_t get_xt(int num);

#endif