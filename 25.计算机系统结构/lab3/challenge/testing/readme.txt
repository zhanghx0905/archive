用自编的测例 test1 测试实现的正确性

在循环体后打断点，得到的缓存中恰有 64 个目标数据。
页大小为 4 KB = 2^12 B
从数组虚地址可以推断 set 的低 6 位，与结果符合。