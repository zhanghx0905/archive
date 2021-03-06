# PA3-2 Hacker Report

#### 算法简述

先将所有长度小于6的密码插入哈希表。每次读入一组密文后，在哈希表中进行查找，并更新弱密码并将其插入哈希表中。

由于所有长度小于6的弱密码已在表内，只需维护长度为6,7,8的三组弱密码。在哈希表中进行插入时，如果发现某两个不同明文对应相同密文，就在相应的表项上做标记；在查找时根据标记输出相应信息。

哈希表采用**开放散列**。

#### 复杂度分析

不超过五位的密码共$b=\sum_{i=0}^{5} 18^i=2000719$个，最坏情况下要插入$3n$个弱口令。哈希表中有不超过$b+3n$个词条。每个输入对应一次查找，三次插入。在平均情况下，认为哈希表单次插入，删除的时间复杂度是$O(1)$的。

时空复杂度均为$O(b+3n)=O(b+n)$.