#include <linux/init.h> // included for __init and __exit macros
#include <linux/io.h>
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/module.h> // included for all kernel modules
#include <linux/slab.h>
#include <linux/uaccess.h>

/* ************** */
/* TASK1: 2018011365 2018011363 */
/* ************** */

#define TEST_SIZE 5

/* TASK1 */
// DONE: choose another 4 ID registers, and find their offsets in documentation
#define MIDR_EL1 0xD00
#define ID_AA64PFR0_EL1 0xD20
#define ID_AA64DFR0_EL1 0xD28
#define ID_AA64ISAR0_EL1 0xD30
#define ID_AA64MMFR0_EL1 0xD38

#define DEBUG_REGISTER_SIZE 0x1000
#define DEBUG_REGISTER_ADDR_CPU0 0x40030000 // this is the correct base address of CPU0

/* TASK1 */
// DONE: find debug register base address of CPU1, CPU2 and CPU3 [Note: default values below are not correct]
#define DEBUG_REGISTER_ADDR_CPU1 0x40032000
#define DEBUG_REGISTER_ADDR_CPU2 0x40034000
#define DEBUG_REGISTER_ADDR_CPU3 0x40036000

typedef uint32_t reg32_t;
typedef uint64_t reg64_t;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LC");
MODULE_DESCRIPTION("Calculate the precise physical address of Raspberry Pi 3B+");

enum CPU {
    CPU0,
    CPU1,
    CPU2,
    CPU3
};

reg64_t val1[TEST_SIZE];
reg64_t val2[TEST_SIZE];

static void access_register_via_system_instruction(void* junk)
{
    reg64_t reg;

    /* TASK1 */
    // DONE: use system instrucion to access 5 ID registers, and store the value to val1[0]~val1[5]
    // hint: asm volatile(...); val1[X] = reg;
    asm volatile("MRS %0,MIDR_EL1"
                 : "=r"(reg)
                 :);
    val1[0] = reg;
    asm volatile("MRS %0,ID_AA64PFR0_EL1"
                 : "=r"(reg)
                 :);
    val1[1] = reg;
    asm volatile("MRS %0,ID_AA64DFR0_EL1"
                 : "=r"(reg)
                 :);
    val1[2] = reg;
    asm volatile("MRS %0,ID_AA64ISAR0_EL1"
                 : "=r"(reg)
                 :);
    val1[3] = reg;
    asm volatile("MRS %0,ID_AA64MMFR0_EL1"
                 : "=r"(reg)
                 :);
    val1[4] = reg;
}

static void access_register_via_memory_map(void* addr)
{
    void __iomem* map_addr = (void __iomem*)addr;

    /* TASK1 */
    // DONE: use memory map to access 5 ID registers, and store the value to val2[0]~val2[5]
    // hint: val2[X] = ioread64(...);
    val2[0] = ioread64(map_addr + MIDR_EL1);
    val2[1] = ioread64(map_addr + ID_AA64PFR0_EL1);
    val2[2] = ioread64(map_addr + ID_AA64DFR0_EL1);
    val2[3] = ioread64(map_addr + ID_AA64ISAR0_EL1);
    val2[4] = ioread64(map_addr + ID_AA64MMFR0_EL1);
}

bool check(void)
{
    int i;
    for (i = 0; i < TEST_SIZE; ++i)
        if (val1[i] != val2[i])
            return false;
    return true;
}

void debug(void)
{
    printk(KERN_INFO "debug: val1[0] = 0x%llx, val1[1] = 0x%llx, val1[2] = 0x%llx, val1[3] = 0x%llx, val1[4] = 0x%llx", val1[0], val1[1], val1[2], val1[3], val1[4]);
    printk(KERN_INFO "debug: val2[0] = 0x%llx, val2[1] = 0x%llx, val2[2] = 0x%llx, val2[3] = 0x%llx, val2[4] = 0x%llx", val2[0], val2[1], val2[2], val2[3], val2[4]);
}

static int __init task1_init(void)
{
    void __iomem* cpu0_addr = ioremap(DEBUG_REGISTER_ADDR_CPU0, DEBUG_REGISTER_SIZE);
    void __iomem* cpu1_addr = ioremap(DEBUG_REGISTER_ADDR_CPU1, DEBUG_REGISTER_SIZE);
    void __iomem* cpu2_addr = ioremap(DEBUG_REGISTER_ADDR_CPU2, DEBUG_REGISTER_SIZE);
    void __iomem* cpu3_addr = ioremap(DEBUG_REGISTER_ADDR_CPU3, DEBUG_REGISTER_SIZE);

    printk(KERN_INFO "module: check CPU0");
    smp_call_function_single(CPU0, access_register_via_system_instruction, NULL, 1);
    smp_call_function_single(CPU1, access_register_via_memory_map, cpu0_addr, 1);
    if (!check()) {
        printk(KERN_INFO "CPU0: base address is not correct");
        debug();
    } else
        printk(KERN_INFO "CPU0: base address is correct");

    printk(KERN_INFO "module: check CPU1");
    smp_call_function_single(CPU1, access_register_via_system_instruction, NULL, 1);
    smp_call_function_single(CPU2, access_register_via_memory_map, cpu1_addr, 1);
    if (!check()) {
        printk(KERN_INFO "CPU1: base address is not correct");
        debug();
    } else
        printk(KERN_INFO "CPU1: base address is correct");

    printk(KERN_INFO "module: check CPU2");
    smp_call_function_single(CPU2, access_register_via_system_instruction, NULL, 1);
    smp_call_function_single(CPU3, access_register_via_memory_map, cpu2_addr, 1);
    if (!check()) {
        printk(KERN_INFO "CPU4: base address is not correct");
        debug();
    } else

        printk(KERN_INFO "CPU2: base address is correct");

    printk(KERN_INFO "module: check CPU3");
    smp_call_function_single(CPU3, access_register_via_system_instruction, NULL, 1);
    smp_call_function_single(CPU0, access_register_via_memory_map, cpu3_addr, 1);
    if (!check()) {
        printk(KERN_INFO "CPU3: base address is not correct");
        debug();
    } else
        printk(KERN_INFO "CPU3: base address is correct");
    printk(KERN_INFO "task1 End");  // 加这一行刷新缓存区
    return 0;
}

static void __exit task1_exit(void)
{
}
module_init(task1_init);
module_exit(task1_exit);