#include <stdint.h>

#define RIP_MAX_ENTRY 25
const uint8_t UDP_PROTOCAL = 0x11;
const uint8_t ICMP_PROTOCAL = 0x01;

typedef struct {
  // 所有字段都是网络字节序（大端序）
  // 没有存储 `family` 字段，因为在请求里是 0，在回应里是 2
  // 也不用存储 `tag` 字段，因为它是 0
  uint32_t addr;
  uint32_t mask;
  uint32_t nexthop;
  uint32_t metric;
} RipEntry;

typedef struct {
  uint32_t numEntries;
  // 下面所有字段都是网络字节序（大端序）
  uint8_t command;
  // 不用存储 `version`，因为它总是 2
  // 不用存储 `zero`，因为它总是 0
  RipEntry entries[RIP_MAX_ENTRY];
} RipPacket;

struct RawRipEntry {
  uint16_t family;  // 0 or 2
  uint16_t tag = 0;
  uint32_t addr;
  uint32_t mask;
  uint32_t next_hop;
  uint32_t metric;
};
struct RawRipPacket {
  uint8_t command;  // 1 or 2
  uint8_t version = 2;
  uint16_t unused = 0;
  RawRipEntry entries[0];
};