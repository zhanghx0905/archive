# 课后作业 Week12

2018011365 张鹤潇

### 第九章 & 第十章

**1 请描述一下DNS基础设施中 stub resolver, public resolver, open resolver，authoritative name server，recursive name server，iterative name server，root name server 之间的关系和区别。**

stub resolver 是本地的名称解析器，它会通过调用某个库来完成名称解析。

open resolver 是公开可访问的名称解析服务器，它能为未指定的IP地址提供递归名称解析。

public resolver 类似 open resolver，未明确查到二者的区别。猜想 public resolver 服务的范围比 open resolver 小。

recursive name server 递归域名服务器是为客户端提供 DNS 服务的中间人，它递归地向根域名服务器和权威域名服务器发送请求，收到包含已请求 IP 地址的权威性域名服务器的响应后，递归解析器将向客户端发送响应。open resolver / public resolver 属于此类。

iterative name server 迭代域名服务器，在不保存所请求 IP 地址时，向查询者返回下一个待查询的 DNS 服务器地址。authoritative name server 和 root name server 都属于此类。

root name server 根域名服务器共 13 个，是递归解析器搜寻 DNS 记录的第一站。它接受包含域名的来自递归解析器的查询，根据扩展名（.com、.net、.org 等），将递归解析器定向到区域权威域名服务器。

authoritative name server 权威域名服务器包含特定域名的信息（如 google.com），向递归解析器响应对这些域名的解析结果。

**2 google的 8.8.8.8域名解析服务器、中国电信的114.114.114.114，对应的是上面的哪一个？是唯一对应吗？为什么？**

对应 public resolver、open resolver 和 recursive name server.

考虑到它们要为大量的用户提供名称解析服务，这些 IP 地址应该对应多台服务器。

**3 在IPv6网络下，有哪些公开可用的域名解析服务器？对比一下与IPv4域名解析服务器（如8.8.8.8,114.114.114.114）访问有什么区别（延迟、包大小、安全机制等）？**

阿里，百度，Google 都提供 IPv6 域名解析服务器。

以 GooglePublic DNS (IPv4：8.8.8.8；IPv6：2001:4860:4860::8844) 为例，在本机逆向查询这两个地址，并抓包分析：

- 二者延迟无明显区别。
- IPv6 的 ANSWER SECTION Type 为 AAAA，查询返回的 IP 地址为 16 字节，而 IPv4 记录 Type 为 A，IP 地址为 4 字节。IPv6 域名服务器同时也返回 IPv4 地址，返回的 DNS 报文更长。
-  二者的安全机制未见差别。

**4 请简述一下SAVA体系结构的三层结构，你认为这种设计合理吗，试简述理由。**

1. 自治域间 IP 源地址验证 (AS)
2. 自治域内 IP 源地址验证 (IP 前缀)
3. 子网内 IP 源地址验证 (端系统)

这种层次结构是按照互联系统的包含关系天然划分的，是合理的，列举这种分层结构的优点如下：

- 可扩展性，具有灵活可变的源地址验证粒度。
- 各层之间可以共同协作，避免层次之间的过度依赖。
- 可演进，与已有协议兼容，自身部署可逐步完成。