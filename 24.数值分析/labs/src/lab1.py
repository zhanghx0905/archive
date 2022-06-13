import time

import matplotlib.pyplot as plt
import numpy as np


def q1():
    h = np.logspace(-16, 0, 100)
    real_err = np.abs(np.cos(1) - (np.sin(1 + h) - np.sin(1)) / h)
    trunc_err = np.sin(1) * h / 2
    rounding_err = 2 * 1e-16 / h
    tot_err = trunc_err + rounding_err

    plt.plot(h, real_err, label='Real error')
    plt.plot(h, trunc_err, ':', label='Truncation error')
    plt.plot(h, rounding_err, ':', label='Rounding error')
    plt.plot(h, tot_err, '--', label='Total error limit')

    plt.xscale('log', base=10)
    plt.yscale('log', base=10)
    plt.xlabel("Step size h")
    plt.ylabel("Error")

    plt.legend()
    plt.show()


def q3():
    dtype = np.float32
    sum_float, n = dtype(0), 0
    while True:
        n += 1
        cur_sum = sum_float + dtype(1 / n)
        if cur_sum == sum_float:
            break
        sum_float = cur_sum
    print(f"对于IEEE单精度浮点数，n = {n} 时求和结果不再变化")

    sum_double = sum(1 / i for i in range(1, n + 1))
    print(f"绝对误差: {sum_float-sum_double}")
    print(f"相对误差: {(sum_float-sum_double)/sum_double}")

    start = time.perf_counter()
    n = int(1e8)
    sum(1 / i for i in range(1, n + 1))
    elapsed = time.perf_counter() - start
    print(f"累加到 1e8 用时: {elapsed} s")


if __name__ == '__main__':
    q1()
    q3()
