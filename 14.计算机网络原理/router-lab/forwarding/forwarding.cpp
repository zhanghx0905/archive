#include <arpa/inet.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdlib.h>
// 在 checksum.cpp 中定义
extern bool validateIPChecksum(uint8_t* packet, size_t len);

/**
 * @brief 增量式修改检验和. Refer to https://www.cnblogs.com/CasperWu/articles/4541904.html
 * @param old_csum 旧检验和
 * @param old_field 数据包中某个域(16 bit)修改前的值
 * @param new_field 数据包中某个域(16 bit)修改后的值
 * @return 新校验和
*/
inline uint16_t csum_incremental_update_modified(uint16_t old_csum, uint16_t old_field, uint16_t new_field)
{
    uint csum = (~old_csum & 0xFFFF) + (~old_field & 0xFFFF) + new_field;
    csum = (csum >> 16) + (csum & 0xFFFF);
    csum += (csum >> 16);
    return ~csum;
}

/**
 * @brief 进行转发时所需的 IP 头的更新：
 *        你需要先检查 IP 头校验和的正确性，如果不正确，直接返回 false ；
 *        如果正确，请更新 TTL 和 IP 头校验和，并返回 true 。
 *        你可以调用 checksum 题中的 validateIPChecksum 函数，
 *        编译的时候会链接进来。
 * @param packet 收到的 IP 包，既是输入也是输出，原地更改
 * @param len 即 packet 的长度，单位为字节
 * @return 校验和无误则返回 true ，有误则返回 false
 */
bool forward(uint8_t* packet, size_t len)
{
    if (!validateIPChecksum(packet, len)) {
        return false;
    }
    iphdr* hdr = (iphdr*)packet;
    uint16_t oldTTL = hdr->ttl << 8, checkSum = hdr->check;
    hdr->ttl--;
    uint16_t newTTL = hdr->ttl << 8;
    hdr->check = htons(csum_incremental_update_modified(ntohs(checkSum), oldTTL, newTTL));
    return true;
}
