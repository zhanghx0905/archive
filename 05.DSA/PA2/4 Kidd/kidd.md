# PA2-4-2 Kidd Report

2018011365 张鹤潇

#### 算法简述

预读入所有输入。将输入的线段端点进行排序，去重后离散化，使一个区间对应于线段树的一个叶子节点。

举例说明离散化的效果，如果输入的线段端点为{1,10,100}，则离散化后，线段树叶子节点存储的区间是{[1,1],[2,9],[10,10],[11,99],[100,100]}.

用线段树进行区间更新，区间查询操作。为了控制时间复杂度，上层节点执行区间更新操作后，不立即更新其子节点，而是打上一个**懒惰标记**`，等查询到子节点时，再对子节点的信息做相应的更新。

#### 困难及其解决

由于n的范围过大，无法直接以每个数为节点建立线段树，必须将输入区间离散化，将空间复杂度从O(n)压缩到O(m).

对于此前从未接触过线段树的我来说，搞明白如何进行离散化是一件困难的事。特别感谢杨卓毅小助教的耐心指导。

#### 复杂度分析

离散化后，线段树的空间复杂度为O(m).

用线段树进行区间更新，区间查询，每次操作时间复杂度O(logm)，总时间复杂度O(mlogm).