# DSA MOOC

### PA1

##### A.Range

思路简明，先对所有点进行排序。再对输入区间左右端点值进行二分查找，返回不大于端点值且数值最大的点的秩。$O(nlogn)$

注意：对是否有点落在左端点上进行特判。

好久没打二分查找了，调试了很久。

##### B.Zuma

动态序列维护问题，由题意，需要模拟的操作有:

- 顺序访问
- 删除
- 遍历前驱和后继

使用了双向链表.

注意：需要考虑初始序列为空的情况。

```c++
scanf("%[^\n]", in);
//为了提高IO效率，将所有输出存入一个大数组中，等到所有输入结束时全部输出
#define MAX 150000000 //多次测试后取到的较合适的Max。
char output[MAX];
```

考虑到链表在顺序访问时的低效，如果数据规模继续扩大，时限将难以满足。

经查阅资料，发现了**char*+strcpy直接模拟**的方法，速度快了一个数量级。想不通为什么，可能与缓存有关吧。

##### C.Lighthouse

先按照横坐标对所有点进行排序，然后统计纵坐标的逆序对数。最后输出纵坐标的顺序对数，即$\frac{n(n-1)}{2}-逆序对数$. $O(nlogn)$.

本题的难点主要在思路上，相关算法套模板即可。

### PA2

##### A.Train

栈混洗问题。用数组简易地实现了栈；算法应用了贪心思想，$O(n)$.

注意：每一次对中间栈进行push操作后，都要判断中间栈的大小是否超限。

##### B. Proper Rebuild

参考了<https://blog.csdn.net/wangbingcsu/article/details/49495309>

对博主的算法略作改进，将每个节点在后序序列中的位置经预处理记录到一个数组中，节省查找时间。

##### C.TSP

关键路径问题，$O(n)$。用数组简易地实现了队列，以供拓扑排序时使用。

对于每个节点，维护其出度，入度和后继数组。为所有入度为0的节点增加起点，出度为0的点增加终点，从起点到终点关键路径的长度就是不同点间的最短距离。

注意：由于不能使用vector，为了防止内存超限，将所有边预读入后再根据每个点的出度开辟内存。


### PA3

##### A.Broadcast

即判断给定无向图是否可二着色。根据提示对全图进行BFS即可。$O(n+m)$.

##### B.Temprature

kd-tree 参考资料：<https://trinkle23897.github.io/pdf/K-D%20Tree.pdf>

$O(m\sqrt{n})$ .  

##### C.deduplicate

直接哈希即可，MAD的参数完全是乱选的。尝试了open和close两种方案，在此题背景下，二者效率大致相同。

$O(n)$

教训：char*比string快; 慎用指针。

```c++
char in[...],data[...];
scanf("%s",in);
data=in;//有误！！
scanf("%s",in);//此时data的值也改变了.
strcpy(data,in);//正确的做法
```

#### PA4

##### A.Toy

利用康托展开将每个排列映射为一个整数, 状态空间 $m=8!=40320$;

方案一: 从每次输入的起始状态开始BFS, $O(Nm)$, 得到了85分.

经过反思, 该方案低效的原因是没有利用前一次搜索遗留的信息. 

方案二:

考虑到m不大, 逆向思维, 从终状态开始逆向搜索整个状态空间, $O(m)$. 

经测试，在本题条件下, 由终状态出发，整个搜索空间都能被遍历到，不存在无法还原的状态.

##### B.Schedule

建立小根堆(完全二叉堆)模拟即可, $O(mlgn)$

卡常数题, 注意细节.

注意: 0 < 每个任务的初始优先级 < 2^32, 需要用unsigned int存储.

##### C.Cycle

A和B可以循环移位得到彼此的充要条件是：

 $|A|=|B|$ 且B是AA的子串. 故套用字符串匹配算法即可.

不知道为啥，套用示例代码中的KMP算法一直出bug, 后来发现了strstr函数, 又好又快。
