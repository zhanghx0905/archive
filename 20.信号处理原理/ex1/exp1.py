import math
import os

import imageio
import matplotlib.patches as patches
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import cm
from scipy.integrate import quad

# TODO: 1. Change N_Fourier to 2, 4, 8, 16, 32, 64, 128, get visualization
# results with differnet number of Fourier Series
N_Fouriers = [2, 4, 8, 16, 32, 64, 128]
N_Fourier = 64

# TODO: optional, implement visualization for semi-circle
# square or semicircle
signal_name = "semicircle"


# TODO: 2. Please implement the function that calculates the Nth fourier coefficient
# Note that n starts from 0
# For n = 0, return a0; n = 1, return b1; n = 2, return a1; n = 3, return b2; n = 4, return a2 ...
# n = 2 * m - 1(m >= 1), return bm; n = 2 * m(m >= 1), return am.
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


# TODO: 3. implement the signal function
def square_wave(t):
    return 0.5 * np.sign(np.sin(t)) + 0.5


# TODO: optional. implement the semi circle wave function
def semi_circle_wave(t):
    assert t >= 0
    if 0 <= t <= 2 * np.pi:
        return np.sqrt(np.pi**2 - (t - np.pi)**2)
    return semi_circle_wave(t - 2 * np.pi)


def function(t):
    if signal_name == "square":
        return square_wave(t)
    elif signal_name == "semicircle":
        return semi_circle_wave(t)
    else:
        raise Exception("Unknown Signal")


def visualize():
    if not os.path.exists(signal_name):
        os.makedirs(signal_name)

    frames = 100

    # x and y are for drawing the original function
    x = np.linspace(0, 2 * math.pi, 1000)
    y = np.zeros(1000, dtype=float)
    for i in range(1000):
        y[i] = function(x[i])

    for i in range(frames):
        figure, axes = plt.subplots()
        color = iter(cm.rainbow(np.linspace(0, 1, 2 * N_Fourier + 1)))

        time = 2 * math.pi * i / 100
        point_pos_array = np.zeros((2 * N_Fourier + 2, 2), dtype=float)
        radius_array = np.zeros((2 * N_Fourier + 1), dtype=float)

        point_pos_array[0, :] = [0, 0]
        radius_array[0] = fourier_coefficient(0)
        point_pos_array[1, :] = [0, radius_array[0]]

        circle = patches.Circle(point_pos_array[0], radius_array[0], fill=False, color=next(color))
        axes.add_artist(circle)

        f_t = function(time)
        for j in range(N_Fourier):
            # calculate circle for a_{n}
            radius_array[2 * j + 1] = fourier_coefficient(2 * j + 1)
            point_pos_array[2 * j + 2] = [point_pos_array[2 * j + 1][0] + radius_array[2 * j + 1] * math.cos((j + 1) * time),   # x axis
                                          point_pos_array[2 * j + 1][1] + radius_array[2 * j + 1] * math.sin((j + 1) * time)]     # y axis
            circle = patches.Circle(
                point_pos_array[2 * j + 1], radius_array[2 * j + 1], fill=False, color=next(color))
            axes.add_artist(circle)

            # calculate circle for b_{n}
            radius_array[2 * j + 2] = fourier_coefficient(2 * j + 2)
            point_pos_array[2 * j + 3] = [point_pos_array[2 * j + 2][0] + radius_array[2 * j + 2] * math.sin((j + 1) * time),   # x axis
                                          point_pos_array[2 * j + 2][1] + radius_array[2 * j + 2] * math.cos((j + 1) * time)]     # y axis
            circle = patches.Circle(
                point_pos_array[2 * j + 2], radius_array[2 * j + 2], fill=False, color=next(color))
            axes.add_artist(circle)

        plt.plot(point_pos_array[:, 0], point_pos_array[:, 1], 'o-')
        plt.plot(x, y, '-')
        plt.plot([time, point_pos_array[-1][0]], [f_t, point_pos_array[-1][1]], '-', color='r')
        plt.gca().set_aspect('equal', adjustable='box')
        plt.savefig(os.path.join(signal_name, "{}.png".format(i)))
        # plt.show()
        plt.close(figure)

    images = []
    for i in range(frames):
        images.append(imageio.imread(os.path.join(signal_name, "{}.png".format(i))))
    imageio.mimsave(f'{signal_name}-{N_Fourier}.mp4', images)


if __name__ == "__main__":
    for signal_name in ['square', 'semicircle']:
        for N_Fourier in N_Fouriers:
            visualize()
            print(f'{signal_name}-{N_Fourier} generated.')
