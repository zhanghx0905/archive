# 实验报告：傅里叶级数的可视化

2018011365 张鹤潇

### 实验原理

对于周期为 $T_1$ 的函数$f(t)$, 其三角傅里叶级数展开
$$
\begin{aligned}
g(t)&=a_0+\sum_{n=1}^\infty (a_n\cos{n\omega_1t}+b_n\sin{n\omega_1t})\\
a_0&=\frac{1}{T_1}\int_0^{T_1}g(t)\text{d}t\\
\forall n\ge1,a_n&=\frac{2}{T_1}\int_0^{T_1}g(t)\cos{n\omega_1t}\text{d}t\\
b_n&=\frac{2}{T_1}\int_0^{T_1}g(t)\sin{n\omega_1t}\text{d}t
\end{aligned}
$$
对于周期$T_1=2\pi$的方波$f(t)=0.5\mathop{sgn}(\sin{t})+0.5$,  $\omega_1=1$, 计算可得
$$
a_0=0.5,a_n=0,b_n=\frac{1-\cos{n\pi}}{n\pi}
$$
对于半圆波信号 $f(t)=\sqrt{\pi^2-(t-\pi)^2},t\in[0,2\pi]$, 也可以根据三角级数公式用数值积分得到其傅里叶系数。

### 实验过程

用 `scipy.integrate` 库完成数值积分功能，相关代码如下：

```python
from scipy.integrate import quad

def fourier_coefficient(n):
    if n == 0:  # a0
        def fun(t): return function(t) / (2 * np.pi)
    else:
        m = (n + 1) // 2
        if n % 2 == 0:    # am
            def fun(t): return function(t) * np.cos(m * t) / np.pi
        else:   # bm
            def fun(t): return function(t) * np.sin(m * t) / np.pi
    return quad(fun, 0, 2 * np.pi, limit=100)[0]
```

补全方波和半圆波函数值计算的代码，改变傅里叶级数的项数，运行脚本进行可视化。实验结果见同目录下的 mp4 文件。

结果表明，随着傅里叶级数展开项的增加，$\tilde g(t)=a_0+\sum_{n=1}^N (a_n\cos{n\omega_1t}+b_n\sin{n\omega_1t})$ 越来越近似等于 $g(t)$.