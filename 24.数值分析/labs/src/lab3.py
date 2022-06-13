import numpy as np


def make_hilbert(n: int) -> np.ndarray:
    ret = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            ret[i, j] = 1 / (i + j + 1)
    return ret


def calculate(n: int, permute=False):
    def cholesky(mat):
        res = mat.copy()
        n = mat.shape[0]
        for i in range(n):
            res[i, i] = np.sqrt(res[i, i] - np.sum(res[i, :i]**2))
            for j in range(i + 1, n):
                res[j, i] = (res[j, i] - np.sum(res[j, :i]
                             * res[i, :i])) / res[i, i]
            res[:i, i] = 0
        return res

    def solve_front(mat, b):
        res = b.copy()
        for i in range(len(res)):
            res[i] /= mat[i, i]
            res[(i + 1):] -= mat[(i + 1):, i:(i + 1)] * res[i]
        return res

    def solve_back(mat, b):
        res = b.copy()
        for i in reversed(range(len(res))):
            res[i] /= mat[i, i]
            res[:i] -= mat[:i, i:(i + 1)] * res[i]
        return res

    hil = make_hilbert(n)
    x = np.ones((n, 1))
    b = hil @ x
    if permute:
        b += 1e-7

    L = cholesky(hil)
    y = solve_front(L, b)
    x_hat = solve_back(L.T, y)

    r_norm = np.linalg.norm(b - hil @ x_hat, np.inf)
    delta_x_norm = np.linalg.norm(x_hat - x, np.inf)
    print(f"|r| = {r_norm:.10f}, |delta x| = {delta_x_norm:.10f}")


for n in [10, 8, 12]:
    for permute in [False, True]:
        print(f"n = {n}, permute = {permute}")
        calculate(n, permute)
