# 1 A+B problem report

2018011365 张鹤潇 计84

#### 算法简述

我使用两种算法实现了本题，简述如下:

##### FFT(final version)

将大整数看成是$x=10$情形下的一个多项式，用$FFT$将系数表达式转化成点值表达式。利用点值表达式的性质，在$O(n)$的时间内完成多项式乘法，再用$DFFT$将点值表达式转换回来，得到最终的结果。

考虑到FFT算法的常数过大，使用所谓的`蝴蝶操作`将递归转化成迭代。

##### 压位乘法

模拟竖式乘法，用longlong数组压八位。

#### 问题及其解决

在以下网站学习了FFT:

https://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2
<https://www.luogu.org/blog/ACdreamer/Fast-Fourier-Transformation>

FFT不太好理解, 相比之下压位简单又好写，而且快.

#### 复杂度分析

记$N=max\{len(a),len(b)\}$，

储存输入，中间值和结果，空间复杂度$O(N)$.

每次FFT的时间复杂度为$O(NlogN)$, 总时间复杂度$O(nNlogN)$.

每次压位乘法的时间复杂度$O(\frac{N^2}{64})$, 总计$O(\frac{nN^2}{64})$.

#### 补充

在本题情境下，压位乘法的速度要比FFT快近十倍，但就渐进意义而言，FFT是$O(NlogN)$级别的算法, 而压位是$O(N^2/64)$的。解释如下：

相比压位乘法，FFT的常数过大，在本题中$N=max\{len(a),len(b)\}\le 5000$并不大，FFT的优势体现不出来。