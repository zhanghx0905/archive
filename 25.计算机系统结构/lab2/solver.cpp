#include "solver.h"
#include "cache.h"
#include <iostream>

int Solver::getAns(BaseCache *cache, int addresses, int lines)
{
    // DONE：获取并返回未知地址
    cache->flushAny();
    cache->accessu();
    int base;
    for (base = 0; base < lines; base++)
    {
        if (cache->access(base) == 0)
            // 如果命中，说明找到 u
            return base;
        if (cache->accessu() == 1)
            // 如果 u 被替换，说明 u 和 base 在同一缓存行
            break;
    }
    base += lines;
    for (; base < addresses; base += lines)
    { // 尝试 u 所在缓存行的所有可能地址
        if (cache->flush(base) == 1)
            return base;
    }
    return -1;
}
