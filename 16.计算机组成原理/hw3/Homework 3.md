# Homework 3

张鹤潇 2018011365

#### 1-5.

C B C D C

#### 6.

独占总线 周期窃取

#### 7.

256 G, 384 G

#### 8.

|数据存储单位以 2 为底，带宽单位以 10 为底。

172.8 MB/s

$800\times 600 \times 3\ \text{byte}\times 60\ \text{Hz}/50\%=172.8\ \text{MB/s}$

#### 9.

3.75 MB/s

1.5 MB/s

4.096 GB/S
$$
\begin{aligned}
\frac{500\ \text{MHz}\times 2\ \text{B}}{200/0.75}
&=3.75\ \text{MB/s}\\
\frac{500\ \text{MHz}\times 2\ \text{B}}{(300+200)/0.75}&
=1.5\ \text{MB/s}\\
\frac{500\ \text{MHz}\times 4\ \text{kB}}{300/0.75+100}&
=4.096\ \text{GB/s}
 \end{aligned}
$$

#### 10.

磁盘访问时间分为寻道时间、旋转延迟、数据传输时间等几部分，而寻道、旋转操作带来的额外开销占总开销的比例比较大；相比随机读取，顺序读取能有效降低这些额外开销，提高性能。