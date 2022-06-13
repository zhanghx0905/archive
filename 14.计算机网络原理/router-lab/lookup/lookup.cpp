#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

#include "router.h"

std::map<ADDR_LEN_PAIR, RoutingTableEntry> routingTable;

/**
 * @brief 插入/删除一条路由表表项
 * @param insert 如果要插入则为 true ，要删除则为 false
 * @param entry 要插入/删除的表项
 *
 * 插入时如果已经存在一条 addr 和 len 都相同的表项，则替换掉原有的。
 * 删除时按照 addr 和 len **精确** 匹配。
 */
void update(bool insert, RoutingTableEntry entry)
{
    ADDR_LEN_PAIR index = { entry.addr, entry.len };
    if (insert) {
        routingTable[index] = entry;
    } else {
        auto iter = routingTable.find(index);
        if (iter != routingTable.end())
            routingTable.erase(iter);
    }
}

/**
 * @brief 进行一次路由表的查询，按照最长前缀匹配原则
 * @param addr 需要查询的目标地址，网络字节序
 * @param nexthop 如果查询到目标，把表项的 nexthop 写入
 * @param if_index 如果查询到目标，把表项的 if_index 写入
 * @return 查到则返回 true ，没查到则返回 false
 */
bool prefix_query(uint32_t addr, uint32_t* nexthop, uint32_t* if_index)
{
    for (int i = 32; i >= 0; --i) {
        uint32_t maskedAddr = addr & LEN_TO_MASK_BE[i]; // 最长前缀匹配
        auto iter = routingTable.find(ADDR_LEN_PAIR(maskedAddr, i));
        if (iter != routingTable.end()) {
            *if_index = iter->second.if_index;
            *nexthop = iter->second.nexthop;
            return true;
        }
    }
    *nexthop = 0;
    *if_index = 0;
    return false;
}
