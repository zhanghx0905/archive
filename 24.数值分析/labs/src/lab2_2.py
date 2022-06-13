from scipy.misc import derivative
from scipy.optimize import fsolve


def q2():
    def solve(f, x0):
        # damping newton method
        epsilon = 1e-8
        lamda0 = 0.9
        x, x_next = x0, x0 + 1
        def df(x): return derivative(f, x, 1e-5)
        k = 0
        print(f"Step {k}, x = {x:.10f}, f(x) = {f(x):.10f}")
        while abs(f(x)) > epsilon or abs(x - x_next) > epsilon:
            s = f(x) / df(x)
            x_next = x - s
            lamda = lamda0
            while abs(f(x_next)) >= abs(f(x)):
                x_next = x - lamda * s
                lamda *= 0.5
            x = x_next
            k += 1
            print(f"Step {k}, x = {x:.10f}, f(x) = {f(x):.10f}, lambda = {lamda}")
        print(f"Solution: x = {x:.10f}")
        # fsolve method
        print(f"fsolve solution: {fsolve(f, x0)}")
        return x

    print("Solving x^3 - x - 1 = 0:")
    solve(lambda x: x**3 - x - 1, 0.6)
    print("Solving -x^3 + 5x = 0:")
    solve(lambda x: -x**3 + 5 * x, 1.35)


if __name__ == '__main__':
    q2()
