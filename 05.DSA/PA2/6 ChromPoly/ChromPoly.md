# PA2-6 ChromPoly Report

2018011365 张鹤潇

#### 算法简述

色多项式问题是NP问题，没有可行的高效方法，只能特判剪枝。

进入搜索入口函数`work`后，

- 若图不连通，则将其拆成多个联通支，分别计算色多项式后再相乘。
- 若图为树或环，直接返回结果$P(T_n,t)=t(t-1)^{n-1},P(C_n,t)=(t-1)^n+(-1)^n(t-1)$.
- 处理图中的二度点和满度点。
- 若某点v及其邻域构成完全图，将其删去,$P(G,t)=(v-degv)P(G/v,t)$.
- 找出图中节点的最小度，若其不超过阈值，则在最小度点的邻域中加边，$P(G)=P(G+e)+P(G/e)$.
- 以上条件均不满足,则在选取度最小的两个邻接点减边。$P(G)=P(G-e)-P(G/e)$.

##### 困难及其解决

我将Wikipedia上色多项式的递推式几乎全写了一遍，仍然无法通过case 10，直到学习了加边的思路。

#### 复杂度分析

一般情形下的色多项式问题是NP问题。

根据Wikipedia，在最坏情况下，时间复杂度为$O(1.62^{m+n})$.

由于我使用递归实现搜索，每调用一次`work`函数就多一个递归实例，所以最坏情况下，空间复杂度也是$O(1.62^{m+n})$的。

实际上，程序的时空复杂度是输入敏感的。

