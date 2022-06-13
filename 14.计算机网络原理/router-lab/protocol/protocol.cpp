#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdlib.h>

#include "rip.h"

#define ASSERT(x) \
  if (!(x)) return false;

/*
  在头文件 rip.h 中定义了结构体 `RipEntry` 和 `RipPacket` 。
  你需要从 IPv4 包中解析出 RipPacket 结构体，也要从 RipPacket 结构体构造出对应的
  IP 包。 由于 RIP 包结构本身不记录表项的个数，需要从 IP 头的长度中推断，所以在
  RipPacket 中额外记录了个数。 需要注意这里的地址都是用 **网络字节序（大端序）**
  存储的，1.2.3.4 在小端序的机器上被解释为整数 0x04030201 。
*/

/**
 * @brief 检查掩码是否符合规范
 * @param mask IP掩码，主机序
 */
inline bool isMaskValid(uint32_t mask) {
  bool one = false;
  for (int i = 0; i < 32; i++, mask >>= 1) {
    bool bit = mask & 0x1;
    if (!one && bit) one = true;
    if (one && !bit) return false;
  }
  return true;
}

/**
 * @brief 从接受到的 IP 包解析出 RIP 协议的数据
 * @param packet 接受到的 IP 包
 * @param len 即 packet 的长度
 * @param output 把解析结果写入 *output
 * @return 如果输入是一个合法的 RIP 包，把它的内容写入 RipPacket 并且返回
 * true；否则返回 false
 *
 * IP 包的 Total Length 长度可能和 len 不同，当 Total Length 大于 len
 * 时，把传入的 IP 包视为不合法。 你不需要校验 IP 头和 UDP 的校验和是否合法。
 * 你需要检查 Command 是否为 1 或 2，Version 是否为 2， Zero 是否为 0，
 * Family 和 Command 是否有正确的对应关系（见上面结构体注释），Tag 是否为 0，
 * Metric 是否在 [1,16] 的区间内，
 * Mask 的二进制是不是连续的 1 与连续的 0 组成等等。
 */
bool disassemble(const uint8_t* packet, uint32_t len, RipPacket* output) {
  iphdr* ipHdr = (iphdr*)packet;
  ASSERT(ipHdr->protocol == UDP_PROTOCAL);  // UDP?
  uint16_t totLen = ntohs(ipHdr->tot_len);
  ASSERT(totLen <= len);                   // Total Length > len?
  uint16_t ipHeaderLen = ipHdr->ihl << 2,  // ip header len per byte
      udpDataLen = ntohs(*(uint16_t*)(packet + ipHeaderLen + 4));
  ASSERT(udpDataLen + ipHeaderLen == totLen);

  const RawRipPacket* ripPacket =
      (RawRipPacket*)(packet + ipHeaderLen + 8);  // 8 byte UDP Header
  uint16_t numEntries = (udpDataLen - 12) / 20;   // 20 byte per entry
  ASSERT(numEntries <= RIP_MAX_ENTRY);

  auto command = ripPacket->command;
  ASSERT(command == 1 || command == 2);
  ASSERT(ripPacket->version == 2);
  ASSERT(ripPacket->unused == 0);

  for (int i = 0; i < numEntries; i++) {
    RawRipEntry entry = ripPacket->entries[i];
    uint16_t family = ntohs(entry.family);
    ASSERT(entry.tag == 0);
    ASSERT(((command == 1 && family == 0) || (command == 2 && family == 2)));

    uint32_t metric_host = ntohl(entry.metric);
    ASSERT(metric_host <= 16 && metric_host != 0);
    ASSERT(isMaskValid(ntohl(entry.mask)));

    output->entries[i] = {entry.addr, entry.mask, entry.next_hop, entry.metric};
  }
  output->command = command;
  output->numEntries = numEntries;
  return true;
}

/**
 * @brief 从 RipPacket 的数据结构构造出 RIP 协议的二进制格式
 * @param rip 一个 RipPacket 结构体
 * @param buffer 一个足够大的缓冲区，你要把 RIP 协议的数据写进去
 * @return 写入 buffer 的数据长度
 *
 * 在构造二进制格式的时候，你需要把 RipPacket 中没有保存的一些固定值补充上，包括
 * Version、Zero、Address Family 和 Route Tag 这四个字段 你写入 buffer
 * 的数据长度和返回值都应该是四个字节的 RIP 头，加上每项 20 字节。
 * 需要注意一些没有保存在 RipPacket 结构体内的数据的填写。
 */
uint32_t assemble(const RipPacket* rip, uint8_t* buffer) {
  ASSERT(rip != nullptr);
  RawRipPacket* packet = (RawRipPacket*)buffer;
  packet->command = rip->command;
  packet->version = 2;
  packet->unused = 0;
  for (int i = 0; i < rip->numEntries; i++) {
    RawRipEntry& entry = packet->entries[i];
    entry.family = htons(rip->command == 2 ? 2 : 0);
    entry.tag = 0;
    entry.addr = rip->entries[i].addr;
    entry.mask = rip->entries[i].mask;
    entry.next_hop = rip->entries[i].nexthop;
    entry.metric = rip->entries[i].metric;
  }
  return 4 + 20 * rip->numEntries;
}
