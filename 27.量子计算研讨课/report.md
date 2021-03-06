## 用 Grover 算法求解数独问题

2018011365 张鹤潇

### 问题描述

考虑最简单的数独问题，在一个 $2\times 2$ 方格中填入 $p_1,p_2,p_3,p_4$ 四个数，它们只能为 0 或 1，且要求同行同列的数不相等。

| $p_1$ | $p_2$ |
| ----- | ----- |
| $p_3$ | $p_4$ |

本文将应用 Grover 搜索算法设计量子电路，在不预先知道答案的情况下，求解这个简单的数独问题。

### 电路设计

#### Oracle

以四个 Qubit $|p_1\rangle,|p_2\rangle,|p_3\rangle,|p_4\rangle$ 代表数独的四个位置，一个 Qubit $out$ 用于记录输入是否是数独的解，另外再引入四个 Qubit $|c_1\rangle,|c_2\rangle,|c_3\rangle,|c_4\rangle$ 存储数独成立的条件是否被满足：
$$
c_1=p_1 \oplus p_2\\
c_2=p_3 \oplus p_4\\
c_3=p_1 \oplus p_3\\
c_4=p_2\oplus p_4\\
$$
其中 $|p_1\rangle,|p_2\rangle,|p_3\rangle,|p_4\rangle$ 初始化为均匀叠加态 $|s\rangle$，$|c_1\rangle,|c_2\rangle,|c_3\rangle,|c_4\rangle$ 初始化为 $|0\rangle$，$out$ 初始化为 $|-\rangle$:
$$
|s\rangle=\dfrac{1}{4}\sum_{x\in \{0,1\}^4} |x\rangle
$$

将整个 Oracle 记作酉算子 $U_\omega$，$x\in\{0,1\}^4$ 表示 $p_1$ 到  $p_4$，目标是令：
$$
U_\omega|x\rangle |0\rangle |-\rangle=\begin{cases}
\phantom{-}|x\rangle|0\rangle|-\rangle \quad \text{for} \; |c_1 c_2 c_3 c_4\rangle\neq|1111\rangle \\
-|x\rangle|0\rangle|-\rangle \quad \text{for} \; |c_1 c_2 c_3 c_4\rangle=|1111\rangle \\
\end{cases}
$$
用两个 CNOT 门可以实现异或操作，再用一个 $32\times 32$ 的 Multi-controlled Toffoli 门连接 $|c_1\rangle,|c_2\rangle,|c_3\rangle,|c_4\rangle$ 和 $|out\rangle$, 在 $|c_1 c_2 c_3 c_4\rangle=|1111\rangle$ 反转 $|out\rangle$ 的相位。电路图如下：

![](.\report_pic\output.png)

为了在 Oracle 作用前后，$|c_1c_2c_3c_4\rangle$ 保持 $|0\rangle$ 不变，需要再做一轮异或操作。这样，整个 Oracle 再加上量子寄存器初始化的电路图如下所示：

![](.\report_pic\oracle.png)

#### Amplitude amplification

设计电路对 $|p_1\rangle,|p_2\rangle,|p_3\rangle,|p_4\rangle$ 进行 Amplitude amplification ($U_s$)，具体实现利用了 Multi-controlled Z 门：
$$
\begin{aligned}
U_s&=2|s\rangle\langle s|-I\\
&=H^{\otimes 4} X^{\otimes 4} (MCZ) X^{\otimes 4} H^{\otimes 4}
\end{aligned}
$$
![](.\report_pic\diff.png)

迭代执行 Oracle 和 Amplitude amplification 两轮，最后对  $|p_1\rangle$ 到  $|p_4\rangle$ 进行测量，就完成了整个量子电路。

![](.\report_pic\circ.png)

### 实验结果

根据电路编写程序，在 [isQ](http://124.16.138.151/i) 平台上编译并模拟运行，得到的结果为：

```
{'0110': 5, '1001': 5}
```

这确实是该数独问题的所有解。

### 总结

我对深入研究量子计算理论并不感兴趣，于是选择这个简单的数独问题设计实验，以此加深自己对量子算法的认识。这个搜索问题虽然简单，但要用量子门搭建电路求解它却并不容易，这也反映出量子编程现阶段的高门槛和局限性。

### Reference

[1] Lecture 3.3 Grover Algorithm

[2] qiskit Textbook, [Grover's Algorithm](https://qiskit.org/textbook/ch-algorithms/grover.html)

### 附录：isQ 实验源代码

```
Defgate MCT4 = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0];
Defgate MCZ3 = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0;
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1
];
qbit p1, p2, p3, p4;
qbit c1, c2, c3, c4, out;
int x1, x2, x3, x4;
procedure main(){
    p1 = |0>;
    p2 = |0>;
    p3 = |0>;
    p4 = |0>;
    c1 = |0>;
    c2 = |0>;
    c3 = |0>;
    c4 = |0>;
    out = |0>;
    H<p1>;
    H<p2>;
    H<p3>;
    H<p4>;
    X<out>;
    H<out>; 
    
    CX<p1, c1>;
    CX<p2, c1>;
    CX<p3, c2>;
    CX<p4, c2>;
    CX<p1, c3>;
    CX<p3, c3>;
    CX<p2, c4>;
    CX<p4, c4>;
    MCT4<c1, c2, c3, c4, out>;
    CX<p1, c1>;
    CX<p2, c1>;
    CX<p3, c2>;
    CX<p4, c2>;
    CX<p1, c3>;
    CX<p3, c3>;
    CX<p2, c4>;
    CX<p4, c4>;
    
    H<p1>; 
    H<p2>; 
    H<p3>; 
    H<p4>; 
    X<p1>;
    X<p2>;
    X<p3>;
    X<p4>;
    MCZ3<p1, p2, p3, p4>;
    X<p1>;
    X<p2>;
    X<p3>;
    X<p4>;
    H<p1>; 
    H<p2>; 
    H<p3>; 
    H<p4>; 

    CX<p1, c1>;
    CX<p2, c1>;
    CX<p3, c2>;
    CX<p4, c2>;
    CX<p1, c3>;
    CX<p3, c3>;
    CX<p2, c4>;
    CX<p4, c4>;
    MCT4<c1, c2, c3, c4, out>;
    CX<p1, c1>;
    CX<p2, c1>;
    CX<p3, c2>;
    CX<p4, c2>;
    CX<p1, c3>;
    CX<p3, c3>;
    CX<p2, c4>;
    CX<p4, c4>;
    
    H<p1>; 
    H<p2>; 
    H<p3>; 
    H<p4>; 
    X<p1>;
    X<p2>;
    X<p3>;
    X<p4>;
    MCZ3<p1, p2, p3, p4>;
    X<p1>;
    X<p2>;
    X<p3>;
    X<p4>;
    H<p1>; 
    H<p2>; 
    H<p3>; 
    H<p4>; 

    x1 = M[p1];
    x2 = M[p2];
    x3 = M[p3];
    x4 = M[p4];
    print x1;
    print x2;
    print x3;
    print x4;
}
```

