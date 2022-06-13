#ifndef REGISTER_H
#define REGISTER_H

#include <linux/io.h>

/* TASK1 */
// DONE: find debug register base address of CPU3.
// Note: You should pass task1 successfully and change the correct address. The machine will CRASH if you run the code with a wrong base address!
#define DEBUG_REGISTER_ADDR_CPU3             0x40036000 
#define DEBUG_REGISTER_SIZE_CPU3             0x1000

// 0x4003b000 is the base address of the cross trigger interface registers on Core 3
#define CTI_REGISTER_ADDR_CPU3               0x4003b000
#define CTI_REGISTER_SIZE_CPU3               0x1000

// Offsets of debug registers
#define DBGDTRRX_EL1                         0x80
#define EDITR_EL1                            0x84
#define EDSCR_EL1                            0x88
#define DBGDTRTX_EL1                         0x8C
#define EDRCR_EL1                            0x90
#define OSLAR_EL1                            0x300
#define EDLAR_EL1                            0xFB0
#define DBGBCRx_EL1(x)                       (0x408 + 16 * (x))             
#define DBGBVRx_EL1(x)                       (0x400 + 16 * (x)) 
#define DBGBCR_DISABLE                       0x0
#define DBGBCR_ENABLE                        0x41e7

// Bits in EDSCR
#define STATUS                               (0x3f)
#define EL                                   (0x300)
#define ERR                                  (1 <<  6)
#define HDE				                     (1 << 14)
#define SSD                                  (1 << 16)
#define ITE                                  (1 << 24)
#define TXfull                               (1 << 29)
#define RXfull                               (1 << 30)

// Bits in EDRCR
#define CSE                                  (1 <<  2)

// Offsets of cross trigger registers
#define CTICONTROL_EL1                        0x0
#define CTIINTACK_EL1                         0x10
#define CTIAPPPULSE_EL1                       0x1C
#define CTIOUTEN0_EL1                         0xA0
#define CTIOUTEN1_EL1                         0xA4
#define CTITRIGOUTSTATUS_EL1                  0x134
#define CTIGATE_EL1                           0x140

// Bits in CTICONTROL
#define GLBEN                                 (1 <<  0)

// Bits in CTIINTACK
#define ACK0                                  (1 <<  0)
#define ACK1                                  (1 <<  1)

// Bits in CTIAPPPULSE
#define APPPULSE0                             (1 <<  0)
#define APPPULSE1                             (1 <<  1)

// Bits in CTIOUTEN<n>
#define OUTEN0                                (1 <<  0)
#define OUTEN1                                (1 <<  1)

// Bits in CTITRIGOUTSTATUS
#define TROUT0                                (1 <<  0)
#define TROUT1                                (1 <<  1)

// Bits in CTIGATE
#define GATE0                                 (1 <<  0)
#define GATE1                                 (1 <<  1)

// Values of EDSCR.STATUS
#define NON_DEBUG                             0x2
#define HLT_BY_DEBUG_REQUEST                  0x13

typedef uint64_t reg64_t;
typedef uint32_t reg32_t;
typedef uint32_t ins_t;
typedef uint64_t va_t;
typedef uint8_t op_t;

#endif