import numpy as np


def q1():
    def solve(mat: np.ndarray):
        u = np.ones((mat.shape[0], 1))
        lamda = 0
        while True:
            v = mat @ u
            lamda_next = np.max(np.abs(v))

            if np.abs(lamda - lamda_next) < 1e-5:
                break
            u = v / lamda_next
            lamda = lamda_next
        print(f"lambda 1 = {lamda}")
        print(f"eigen vec 1 =\n{u}\n")

    solve(np.array([[5, -4, 1],
                    [-4, 6, -4],
                    [1, -4, 7]]))
    solve(np.array([[25, -41, 10, -6],
                    [-41, 68, -17, 10],
                    [10, -17, 5, -3],
                    [-6, 10, -3, 2]]))


def q23():
    def sign(num): return 1 if num >= 0 else -1

    def qr(mat: np.ndarray):
        n = mat.shape[0]
        q, r = np.eye(n), mat.copy()
        for k in range(n):
            sigma = sign(r[k, k]) * np.sqrt(np.sum(r[k:, k]**2))
            tmp = np.zeros((n - k, 1))
            tmp[0] = 1
            v = r[k:, k:(k + 1)] + sigma * tmp  # (n-k, 1)
            beta = v.T @ v
            w = np.concatenate((np.zeros((k, 1)), v / np.sqrt(beta)))    # (n, 1)
            q = q @ (np.eye(n) - 2 * w @ w.T)
            r[k:, k:] -= 2 / beta * v @ (v.T @ r[k:, k:])
        return q, r

    def basicQR(mat):
        print("Basic QR")
        q, r = qr(mat)
        print(f"iteration 1: mat =\n{r @ q}\n")

    def biasedQR(mat):
        print("Biased QR")
        n = mat.shape[0]
        for i in range(4):
            s = mat[-1, -1]
            q, r = qr(mat - s * np.eye(n))
            mat = r @ q + s * np.eye(n)
            print(f"iteration {i+1}: mat =\n{ mat }")

        print(f"eigenvalues = {np.diag(mat)}")

    mat = np.array([[0.5, 0.5, 0.5, 0.5],
                    [0.5, 0.5, -0.5, -0.5],
                    [0.5, -0.5, 0.5, -0.5],
                    [0.5, -0.5, -0.5, 0.5]])
    basicQR(mat)
    biasedQR(mat)


if __name__ == '__main__':
    q1()
    q23()
