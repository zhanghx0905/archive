# Homework 4

2018011365 张鹤潇

### 4.2

纯 ALOHA 协议的信道利用率最高为 18.4%.
$$
N\frac{1000\ bit}{100\ s}\le 56\ kbps\times18.4\%
$$
得 $N\le 1030$

### 4.13

Ethernet 基于曼彻斯特编码，波形震荡频率是数据传输速率的两倍，波特率 $20\ MBaud$

### 4.14

对于曼彻斯特编码，LH 表示 0，HL 表示 1，输出为 LHLHLHHLHLHLLHHLLHHL

### 4.15

发送方数据发送时间 $\frac{256\ b}{10\ Mbps}=25.6\ \mu s$

数据帧传输时间 $\frac{1\ km}{200\ m/ms}=5\ \mu s$

确认帧发送时间 $\frac{32\ b}{10\ Mbps}=3.2\ \mu s$

确认帧传输时间 $5\ \mu s$

总传输时间 $38.8\ \mu s$

数据传输速率 $\frac{256-32\ b}{38.8\ \mu s}=5.77\ Mbps$

### 4.18

可以将电缆最大长度减小到十分之一。

### 4.25

每帧数据的错误率 $p=1-(1-10^{-7})^{64\times8}=5.12\times 10 ^{-5}$

平均每秒损坏帧数 $\frac{11\ Mbps}{64\times 8\ b}p=1.1$ 

### 4.27

可能该网络：

- 数据重传的成本很高；
- 数据损坏的概率较大。

## IEEE 802.3协议实验 思考题

### 1

<img src="Homework 4_pic/image-20201201144543102.png" alt="image-20201201144543102" style="zoom:80%;" />

该帧由 14 bytes 的以太网首部 (目的MAC、源MAC、类型) 和 28 bytes 的 ARP 数据包组成。

与收到的 60 bytes 长的 ARP 包相比，这个包少了 18 bytes 的填充字段，这是因为 Wireshark 捕获的发出帧是尚未封装完全的。

> [Frame with no padding | Microchip](https://www.microchip.com/forums/m493473.aspx)

### 2

针对网络层协议的 Ethernet type 还有 IPv6（0x86dd) 和 RARP(0x8035).