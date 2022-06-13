# 栈溢出实验

实验很有趣，但 `x86` 汇编我几乎都忘了。

扩展实验参考了 [手把手教你栈溢出从入门到放弃（上）](https://paper.seebug.org/271/)，但成功的关键在于关闭 Ubuntu 的 ASLR，参考了 [Linux：关闭地址随机化](https://blog.csdn.net/weixin_42072280/article/details/89326309)，每次重启虚拟机之后，都要重新关闭 ASLR：

```shell
su
# 输入密码
echo 0 > /proc/sys/kernel/randomize_va_space
```

