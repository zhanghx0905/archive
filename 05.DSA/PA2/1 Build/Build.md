# PA2-1 Build Report

#### 算法简述

用一个结构体数组模拟多叉树。每个节点维护其父亲，左右兄弟，首个孩子，孩子数量，高度，规模，后缀兄弟的最大高度等信息。

移动子树时，对树的高度和规模进行维护。

- 更新Size：由父节点向根节点逐点更新，时间复杂度$O(h)$.
- 更新Height：遍历其左兄弟，更新左兄弟后缀最大高度`sufh`。父节点的高度即为首个孩子的`sufh`。如此，继续遍历父节点的左兄弟，直至到达根节点。时间复杂度$O(cost)$

#### 问题及其解决

在本道题中，我遇到了两个主要的问题。

首先，寻找新父节点之前，需要移除旧子树。否则，若新父节点在旧子树下，就会将子树接到它自己身上，从而破坏树的拓扑结构。经过漫长的debug，我才发现了这个错误。

其次，初始化节点的后缀最大高度`sufh`时，如果使用朴素的递归方法，有可能导致爆栈。我的解决办法是，将节点按照BFS序加入一个栈中，然后将栈中元素逐个弹出，自下而上，按照逆BFS序初始化`sufh`. 

#### 复杂度分析

输入，树，辅助队列和栈的空间复杂度均为$O(n)$，总空间复杂度为$O(n)$.

初始化树的时间复杂度为O(n), 移动子树和查询为$O(\sum cost)$，总时间复杂度$O(n+\sum cost)$.