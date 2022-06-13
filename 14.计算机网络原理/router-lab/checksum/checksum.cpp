#include <arpa/inet.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdlib.h>
/**
 * @brief 进行 IP 头的校验和的验证
 * @param packet 完整的 IP 头和载荷
 * @param len 即 packet 的长度，单位是字节，保证包含完整的 IP 头
 * @return 校验和无误则返回 true ，有误则返回 false
 */
bool validateIPChecksum(uint8_t* packet, size_t len)
{
    iphdr* hdr = (iphdr*)packet;
    uint16_t checkSum = ntohs(hdr->check);

    size_t headerLen = hdr->ihl << 1; // header length per 16 bits
    uint32_t calSum = 0;
    uint16_t* packetU16 = (uint16_t*)packet;
    for (int i = 0; i < headerLen; i++) {
        calSum += ntohs(packetU16[i]);
    }
    calSum = (calSum >> 16) + (calSum & 0xffff) - checkSum;
    calSum += (calSum >> 16);
    return checkSum == (uint16_t)(~calSum);
}
