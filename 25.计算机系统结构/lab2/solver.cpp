#include "solver.h"
#include "cache.h"
#include <iostream>

int Solver::getAns(BaseCache *cache, int addresses, int lines)
{
    // DONE����ȡ������δ֪��ַ
    cache->flushAny();
    cache->accessu();
    int base;
    for (base = 0; base < lines; base++)
    {
        if (cache->access(base) == 0)
            // ������У�˵���ҵ� u
            return base;
        if (cache->accessu() == 1)
            // ��� u ���滻��˵�� u �� base ��ͬһ������
            break;
    }
    base += lines;
    for (; base < addresses; base += lines)
    { // ���� u ���ڻ����е����п��ܵ�ַ
        if (cache->flush(base) == 1)
            return base;
    }
    return -1;
}
