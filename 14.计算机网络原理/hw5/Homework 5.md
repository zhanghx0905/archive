# Homework 5

2018011365 张鹤潇

### 5.2

不是，虚电路网络需要具备将单个数据包从任意源端路由到任意接收方的能力，以完成建立虚电路的操作。

### 5.3

- 滑动窗口大小
- 最大包长
- 数据传输速率

### 5.9

设簇数为 $x$, 区域数为 $y$, 每个区域路由器数 $z$，$xyz\ge 4800$, 枚举求 $x+y+z$ 的最小值。

得 $x=15,y=16, z=20$ 时路由表尺寸最小，其中 $x,y,z$ 可交换。路由表最小尺寸
$$
(x+y+z-2)_{min}=49
$$

因不必存储本 cluster 和本 region 的地址，故结果减 2.

### 5.10

当路由器有发往某移动主机的 IP 包时，它会广播 ARP，询问该移动主机的 802.3 MAC 地址；当移动主机不在其家乡网络时，家乡代理会响应 ARP，使得由器将移动用户的 IP 与 家乡代理的 MAC 地址关联在一起。

### 5.22

不是。如果加速型数据包过多，则延迟可能更大。

### 5.28

主机位为12位，共能容纳 $2^{12}-2=4094 $ 台主机。

### 5.34

只要每个路由器都有单独的 ip 地址，且与同一个连接相关的数据包都通过同一路由器进出，就可以正常工作。

### 5.40

$$
\frac{2^{128}}{10^{6} ps^{-1}}=3.4\times10^{20}\ s\approx1\times10^{13}\ year
$$

### 5.42

需要改变，这种改变是仅限技术上的。因 IP 地址变长了，相应字段也需要变化。

## IPv4 协议 思考题

### 1

IP 数据报的长度超过数据链路的 MTU 时需要分段。

分段可能在源主机上进行，也可能在中间路由器上进行。

分段重组可能在中间路由器上（对其它网络透明），也可能在目的主机上（对其它网络不透明）.

### 2

包括：

- 安全性
- 松散源路由，给出一连串路由器接口的 IP 地址。IP 包必须沿着这些 IP 地址传送，但允许在相继的两个 IP 地址之间跳过多个路由器。
- 严格源路由，给出一连串路由器接口的 IP 地址。IP 包必须沿着这些地址传送，如果下一跳不在 IP 地址表中，则发生错误。
- 记录路径，当 IP 包离开每个路由器时，记录路由器出站接口的 IP 地址。
- 时间戳
- Stream ID，携带流标识符 (Stream identifier).

## ICMPv4 协议 思考题

### 1

填充空间，从而让后续的 “Internet Header 64 bits of Original Data Datagram” 的位置相同.

### 2

> [ICMP攻击和基于ICMP的路由欺骗技术](https://www.20cn.net/ns/wz/net/data/20010406113851.htm)

- ICMP攻击导致拒绝服务（DoS）
- ICMp 重定向（redirect）路由欺骗

## ARP 协议 思考题

### 1

当主机发起跨网段 ARP 请求时，路由器将自己的 MAC 地址通过 ARP 返回给发送者，从而实现对网段外 IP 的 MAC 地址代理。

抓包结果显示，此时路由器的响应与其它正常 ARP 回复并无区别。

### 2

> [ Address Resolution Protocol (ARP) caching behavior in TCP/IP implementations for Win10](https://docs.microsoft.com/en-us/troubleshoot/windows-server/networking/address-resolution-protocol-arp-caching-behavior)

我的 OS 版本是 Windows 10 20H2. 根据文档，ARP 动态表项的存活时间，即”可访问时间“计算公式：
$$
\text{可访问时间}=\text{基本可访问时间}\times k,\ k \in [0.5,1.5]
$$
在命令行下输入指令 `netsh interface ipv4 show interface 21` 检查可访问时间：

<img src="Homework 5_pic/image-20201221232546323.png" alt="image-20201221232546323" style="zoom:67%;" />



通过 `netsh interface ipv4 set interface 21 basereachable=60000` 可配置基本可访问时间，进而影响可访问时间。

<img src="Homework 5_pic/image-20201221232913602.png" alt="image-20201221232913602" style="zoom:67%;" />

更改后，观察到 ARP 缓存删除时间改变。

### 3

> https://www.practicalnetworking.net/series/arp/arp-probe-arp-announcement/

我在局域网中更改本机的 IP，没有观察到无偿 ARP，而观察到了三次 ARP Probe 和一次 ARP Announcement.

<img src="Homework 5_pic/image-20201229212803295.png" alt="image-20201229212803295" style="zoom:67%;" />

ARP Probe 报文向局域网广播，检查当前 IP 是否已被使用；它不会更改局域网内其他设备的 ARP 表，因为报文的 Sender IP 和 Target MAC 都被设为了 0.

主机没有收到来自可能已在使用当前 IP 地址的任何人的响应，于是发送 ARP  Announcement 在局域网内声明当前 IP 已被本机使用。