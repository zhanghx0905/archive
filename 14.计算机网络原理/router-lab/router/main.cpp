#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iostream>

#include "rip.h"
#include "router.h"
#include "router_hal.h"

extern bool validateIPChecksum(uint8_t* packet, size_t len);
extern void update(bool insert, RoutingTableEntry entry);
extern bool prefix_query(uint32_t addr, uint32_t* nexthop, uint32_t* if_index);
extern bool forward(uint8_t* packet, size_t len);
extern bool disassemble(const uint8_t* packet, uint32_t len, RipPacket* output);
extern uint32_t assemble(const RipPacket* rip, uint8_t* buffer);

uint8_t packet[2048];
uint8_t output[2048];

// for online experiment, don't change
#ifdef ROUTER_R1
// 0: 192.168.1.1
// 1: 192.168.3.1
// 2: 192.168.6.1
// 3: 192.168.7.1
const in_addr_t addrs[N_IFACE_ON_BOARD] = { 0x0101a8c0, 0x0103a8c0, 0x0106a8c0,
    0x0107a8c0 };
#elif defined(ROUTER_R2)
// 0: 192.168.3.2
// 1: 192.168.4.1
// 2: 192.168.8.1
// 3: 192.168.9.1
const in_addr_t addrs[N_IFACE_ON_BOARD] = { 0x0203a8c0, 0x0104a8c0, 0x0108a8c0,
    0x0109a8c0 };
#elif defined(ROUTER_R3)
// 0: 192.168.4.2
// 1: 192.168.5.2
// 2: 192.168.10.1
// 3: 192.168.11.1
const in_addr_t addrs[N_IFACE_ON_BOARD] = { 0x0204a8c0, 0x0205a8c0, 0x010aa8c0,
    0x010ba8c0 };
#else

// 自己调试用，你可以按需进行修改，网络序
// 0: 10.0.0.1
// 1: 10.0.1.1
// 2: 10.0.2.1
// 3: 10.0.3.1
in_addr_t addrs[N_IFACE_ON_BOARD] = { 0x0100000a, 0x0101000a, 0x0102000a,
    0x0103000a };
#endif

const in_addr_t RIP_MULITCAST_ADDR = 0x090000e0; // RIP 组播 224.0.0.9 网络序
const macaddr_t RIP_MULITCAST_MAC = { 0x01, 0x00, 0x5e, 0x00, 0x00, 0x09 };

#define IP_FMT(x) x >> 24, x >> 16 & 0xFF, x >> 8 & 0xFF, x & 0xFF
#define OUTPUT 0 // 测试输出

/**
 * @brief 将路由表中输出到 stdout
*/
void print_table()
{
    int size = routingTable.size();
    printf("routing table: size = %d, items = [", size);
    for (auto item : routingTable) {
        auto e = item.second;
        printf(
            "  { addr: %d.%d.%d.%d, len: %x, nexthop: %d.%d.%d.%d, if_index: "
            "%d, metric: %x },\n",
            IP_FMT(e.addr), e.len, IP_FMT(e.nexthop), e.if_index, e.metric);
    }
    printf("]\n");
}

// 计算校验和，返回值为网络序
uint16_t cal_checksum(uint16_t* pkt, size_t bytes)
{
    uint32_t sum = 0;
    bytes >>= 1;
    for (int i = 0; i < bytes; i++) {
        sum += ((uint16_t*)pkt)[i];
    }
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

/**
 * @brief 计算ip头的checksum并设置之
*/
void cal_ip_checksum(iphdr* ipHdr)
{
    ipHdr->check = 0; // ihl 单位是 4 bytes
    ipHdr->check = cal_checksum((uint16_t*)ipHdr, 4 * ipHdr->ihl);
}

/**
 * @brief 计算icmp的checksum并设置之
 * @param head_len icmp 包长度, 单位 byte
*/
void cal_icmp_checksum(icmp* icmp_msg, size_t head_len)
{
    icmp_msg->icmp_cksum = 0;
    icmp_msg->icmp_cksum = cal_checksum((uint16_t*)icmp_msg, head_len);
}

void send_response(
    const in_addr_t dst_addr,
    const macaddr_t dst_mac,
    uint32_t interface = -1u,
    uint8_t* pkt = packet)
{
    iphdr* ipHdr = (iphdr*)pkt;
    *ipHdr = iphdr {
        .ihl = 5,
        .version = 4,
        .tos = 0,
        .tot_len = 0, // set later
        .id = 0,
        .frag_off = 0,
        .ttl = 1,
        .protocol = UDP_PROTOCAL,
        .check = 0, // set later
        .saddr = 0, // set later
        .daddr = dst_addr,
    };

    udphdr* udpHdr = (udphdr*)(pkt + 20);
    *udpHdr = udphdr {
        htons(520), htons(520), // src&dst port for rip
        0, // len, set later
        0 // set checksum to 0
    };

#define SEND()                                                          \
    {                                                                   \
        rip.numEntries = cnt;                                           \
        uint16_t tot_len = (uint16_t)assemble(&rip, pkt + 28) + 20 + 8; \
        ipHdr->tot_len = htons(tot_len);                                \
        cal_ip_checksum(ipHdr);                                         \
        udpHdr->len = htons(tot_len - 20);                              \
        HAL_SendIPPacket(if_index, pkt, tot_len, dst_mac);              \
    }
    RipPacket rip;
    rip.command = 2; //response
    for (uint32_t if_index = 0; if_index < N_IFACE_ON_BOARD; if_index++) {
        if (interface == -1u || if_index == interface) {
            ipHdr->saddr = addrs[if_index];
            size_t cnt = 0;
            for (auto item : routingTable) {
                auto entry = item.second;
                rip.entries[cnt++] = {
                    .addr = entry.addr,
                    .mask = LEN_TO_MASK_BE[entry.len],
                    .nexthop = 0, // entry.nexthop,
                    // split horizon with reverse poisoning
                    .metric = htonl(entry.if_index == if_index ? 16u : entry.metric)
                };
                if (cnt >= RIP_MAX_ENTRY) { // RIP Response > 25 条, 拆分发送
                    SEND();
                    cnt = 0;
                }
            }
            if (cnt > 0) { // 发送剩下部分
                SEND();
            }
        }
    }
}

void send_icmp(uint8_t icmp_type, uint8_t icmp_code,
    int if_index, in_addr_t src_addr, macaddr_t src_mac)
{
    // append ip header and more 8 bytes from original datagram
    memcpy(output + 28, packet, 28 * sizeof(uint8_t));
    uint16_t totlen = 56; // 20 (ip) + 8 (icmp) + original payload (20 + 8)
    iphdr* ipHdr = (iphdr*)output;
    *ipHdr = iphdr {
        .ihl = 5,
        .version = 4,
        .tos = 0,
        .tot_len = htons(totlen),
        .id = 0,
        .frag_off = 0,
        .ttl = 64,
        .protocol = ICMP_PROTOCAL,
        .check = 0, // set later
        .saddr = addrs[if_index],
        .daddr = src_addr,
    };
    cal_ip_checksum(ipHdr);

    struct icmphdr* icmp_header = (struct icmphdr*)&output[20];
    icmp_header->type = icmp_type;
    icmp_header->code = icmp_code;
    *(uint32_t*)(output + 24) = 0; // fill unused fields with zero
    cal_icmp_checksum((icmp*)icmp_header, totlen - 20);
    HAL_SendIPPacket(if_index, output, totlen, src_mac);

#if OUTPUT
    fprintf(stderr, "ICMP sended\n");
    for (int i = 0; i < totlen; i++)
        fprintf(stderr, "%02hhx ", output[i]);
    fprintf(stderr, "\n");
#endif
}

int main(int argc, char* argv[])
{
    // 0a.
    int res = HAL_Init(OUTPUT, addrs);
    if (res < 0) {
        return res;
    }

    // 0b. Add direct routes
    // For example:
    // 10.0.0.0/24 if 0
    // 10.0.1.0/24 if 1
    // 10.0.2.0/24 if 2
    // 10.0.3.0/24 if 3
    for (uint32_t i = 0; i < N_IFACE_ON_BOARD; i++) {
        RoutingTableEntry entry = {
            .addr = addrs[i] & 0x00FFFFFF, // network byte order
            .len = 24, // host byte order
            .if_index = i, // host byte order
            .nexthop = 0, // network byte order, means direct
            .metric = 1, // 主机序
        };
        update(true, entry);
    }

    uint64_t last_time = 0;
    while (1) {
        uint64_t time = HAL_GetTicks();
        // the RFC says 30s interval,
        // but for faster convergence, use 5s here
        if (time > last_time + 5 * 1000) {
            // ref. RFC 2453 Section 3.8
            printf("5s Timer\n");
            // HINT: print complete routing table to stdout/stderr for debugging
            // DONE: send complete routing table to every interface
            printf("send complete routing table to every interface per 5s\n");
#if OUTPUT
            print_table();
#endif
            send_response(RIP_MULITCAST_ADDR, RIP_MULITCAST_MAC);
            last_time = time;
        }

        int mask = (1 << N_IFACE_ON_BOARD) - 1;
        macaddr_t src_mac;
        macaddr_t dst_mac;
        int if_index;
        res = HAL_ReceiveIPPacket(mask, packet, sizeof(packet), src_mac, dst_mac,
            1000, &if_index);
        if (res == HAL_ERR_EOF) {
            break;
        } else if (res < 0) {
            return res;
        } else if (res == 0) {
            // Timeout
            continue;
        } else if (res > sizeof(packet)) {
            // packet is truncated, ignore it
            continue;
        }

        // 1. validate
        if (!validateIPChecksum(packet, res)) {
            printf("Invalid IP Checksum\n");
            // drop if ip checksum invalid
            continue;
        }
        in_addr_t src_addr, dst_addr;
        // DONE: extract src_addr and dst_addr from packet (big endian)
        iphdr* ipHdr = (iphdr*)packet;
        src_addr = ipHdr->saddr, dst_addr = ipHdr->daddr;

        // 2. check whether dst is me
        bool dst_is_me = false;
        for (int i = 0; i < N_IFACE_ON_BOARD; i++) {
            if (memcmp(&dst_addr, &addrs[i], sizeof(in_addr_t)) == 0) {
                dst_is_me = true;
                break;
            }
        }
        // DONE: handle rip multicast address(224.0.0.9)
        dst_is_me = dst_is_me || (memcmp(&dst_addr, &RIP_MULITCAST_ADDR, sizeof(in_addr_t)) == 0);

        if (dst_is_me) {
            // 3a.1
            RipPacket rip;
            // check and validate
            if (disassemble(packet, res, &rip)) {
                if (rip.command == 1) {
                    // 3a.3 request, ref. RFC 2453 Section 3.9.1
                    // **only need to respond to whole table requests** in the lab
                    if (rip.numEntries == 1 && rip.entries[0].metric == htonl(16))
                        send_response(src_addr, src_mac, if_index, output);
                    // DONE: fill resp
                    // implement split horizon with poisoned reverse
                    // ref. RFC 2453 Section 3.4.3
                    // if you don't want to calculate udp checksum, set it to zero
                } else {
                    // 3a.2 response, ref. RFC 2453 Section 3.9.2
                    // DONE: update routing table
                    // new metric = MIN (metric + cost, infinity)
                    // update metric, if_index, nexthop
                    // HINT: handle nexthop = 0 case
                    // HINT: what is missing from RoutingTableEntry?
                    // you might want to use `prefix_query` and `update`, but beware of
                    // the difference between exact match and longest prefix match.
                    for (uint32_t i = 0; i < rip.numEntries; ++i) {
                        auto re = rip.entries[i];
                        uint32_t metric = std::min(ntohl(re.metric) + 1, 16u), // 主机序
                            len = __builtin_popcount(re.mask);
                        uint32_t nexthop = re.nexthop;
                        if (nexthop == 0)
                            nexthop = src_addr; // handle nexthop = 0 case
                        ADDR_LEN_PAIR index = ADDR_LEN_PAIR(re.addr, len); // exact match
                        auto iter = routingTable.find(index);

                        if (iter != routingTable.end()) {
                            auto& entry = iter->second; // 更新已有表项
                            if ((src_addr == entry.nexthop && metric != entry.metric)
                                || metric < entry.metric) {
                                if (metric == 16u) {
                                    routingTable.erase(iter);
                                } else {
                                    entry.nexthop = nexthop;
                                    entry.metric = metric;
                                }
                            }
                        } else { // 插入新路由项
                            if (metric != 16u)
                                routingTable[index] = {
                                    .addr = re.addr,
                                    .len = len,
                                    .if_index = if_index,
                                    .nexthop = src_addr,
                                    .metric = metric,
                                };
                        }
                    }
                    printf("routing table updated.\n");
#if OUTPUT
                    print_table();
#endif
                    // optional: triggered updates ref. RFC 2453 Section 3.10.1
                }
            } else {
                // not a rip packet
                // handle icmp echo request packet
                // DONE: how to determine it is icmp echo request packet

                if (ipHdr->protocol == ICMP_PROTOCAL) {
                    icmp* icmpMsg = (icmp*)(packet + 20);
                    if (icmpMsg->icmp_type == ICMP_ECHO) {
                        printf("received packet is icmp echo request packet\n");
                        // construct icmp echo reply
                        // reply is mostly the same as request,
                        // you need to:
                        iphdr* ip_header = (iphdr*)output;
                        memcpy(ip_header, ipHdr, sizeof(iphdr));
                        icmp* icmp_msg = (icmp*)(output + 20);
                        memcpy(icmp_msg, icmpMsg, sizeof(icmp));

                        // 1. swap src ip addr and dst ip addr
                        std::swap(ip_header->saddr, ip_header->daddr);
                        // 2. change icmp `type` in header
                        icmp_msg->icmp_type = ICMP_ECHOREPLY;
                        // 3. set ttl to 64
                        ip_header->ttl = 64;
                        // 4. re-calculate icmp checksum and ip checksum
                        cal_ip_checksum(ip_header);
                        auto totlen = ntohs(ip_header->tot_len);
                        cal_icmp_checksum(icmp_msg, totlen - 20);
                        // 5. send icmp packet
                        HAL_SendIPPacket(if_index, output, totlen, src_mac);
                    }
                } else
                    printf("received packet is not icmp echo request or rip\n");
            }
        } else {
            // 3b.1 dst is not me
            // check ttl
            uint8_t ttl = packet[8];
            if (ttl <= 1u) {
                printf("ttl <= 1, Time Exceeded for src %d.%d.%d.%d dst %d.%d.%d.%d\n", IP_FMT(src_addr),
                    IP_FMT(dst_addr));
                // send icmp time to live exceeded to src addr
                send_icmp(ICMP_TIME_EXCEEDED, 0, if_index, src_addr, src_mac);

            } else {
                // forward
                // beware of endianness
                uint32_t nexthop, dest_if;
                if (prefix_query(dst_addr, &nexthop, &dest_if)) {
                    // found
                    macaddr_t dest_mac;
                    // direct routing
                    if (nexthop == 0) {
                        nexthop = dst_addr;
                    }
                    if (HAL_ArpGetMacAddress(dest_if, nexthop, dest_mac) == 0) {
                        // found
                        memcpy(output, packet, res);
                        // update ttl and checksum
                        forward(output, res);
                        HAL_SendIPPacket(dest_if, output, res, dest_mac);
                    } else {
                        // not found
                        // you can drop it
                        printf("ARP not found for nexthop %x\n", nexthop);
                    }
                } else {
                    // not found
                    // send ICMP Destination Network Unreachable
                    printf("IP not found in routing table for src %d.%d.%d.%d dst %d.%d.%d.%d\n", IP_FMT(src_addr),
                        IP_FMT(dst_addr));
                    // icmp type = Destination Unreachable
                    // icmp code = Destination Network Unreachable
                    send_icmp(ICMP_DEST_UNREACH, ICMP_NET_UNREACH, if_index, src_addr, src_mac);
                }
            }
        }
    }
    return 0;
}
