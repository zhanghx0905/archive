#include <stdint.h>

#include <map>
#include <utility>

/*
  表示路由表的一项。
  约定 addr 和 nexthop 以 **网络字节序（大端序）** 存储。
  这意味着 1.2.3.4 在小端序的机器上被解释为整数 0x04030201 而不是 0x01020304。
  保证 addr 和 len 构成合法的网络前缀。
  当 nexthop 为零时这是一条直连路由。
  你可以在全局变量中把路由表以一定的数据结构格式保存下来。
*/
typedef struct {
  uint32_t addr;      // 网络字节序，IPv4 地址
  uint32_t len;       // 主机字节序，前缀长度
  uint32_t if_index;  // 主机字节序，出端口编号
  uint32_t nexthop;   // 网络字节序，下一跳的 IPv4 地址
  // 为了实现 RIP 协议，需要在这里添加额外的字段
  uint32_t metric; // 主机序
} RoutingTableEntry;

// 前缀长度(主机序)转掩码(网络序)
const uint32_t LEN_TO_MASK_BE[33] = {
    0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff,
    0x80ff, 0xc0ff, 0xe0ff, 0xf0ff, 0xf8ff, 0xfcff, 0xfeff, 0xffff,
    0x80ffff, 0xc0ffff, 0xe0ffff, 0xf0ffff, 0xf8ffff, 0xfcffff, 0xfeffff, 0xffffff,
    0x80ffffff, 0xc0ffffff, 0xe0ffffff, 0xf0ffffff, 0xf8ffffff, 0xfcffffff, 0xfeffffff, 0xffffffff,
};

using ADDR_LEN_PAIR = std::pair<uint32_t, u_int32_t>;
extern std::map<ADDR_LEN_PAIR, RoutingTableEntry> routingTable;