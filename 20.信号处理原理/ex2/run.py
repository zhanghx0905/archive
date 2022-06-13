import os
from typing import List

import librosa
import matplotlib.pyplot as plt
import numpy as np
import soundfile as sf
from scipy import signal


SAMPLE_RATE = 8000
STOP_RATE = 3000
TIME = 30
SPLIT_TIME = 30
NUM_FILES = 4
NUM_PER_WAVE = TIME // SPLIT_TIME
NUM_WAVES = NUM_FILES * NUM_PER_WAVE

os.makedirs('output', exist_ok=True)


def split_audio(audios: List[np.ndarray]):
    splited = []
    for audio in audios:
        splited.extend(np.split(audio, NUM_PER_WAVE))
    return splited


def merge_audio(audios: List[np.ndarray]):
    merged = []
    idx = 0
    while idx < len(audios):
        tmp = []
        for _ in range(NUM_PER_WAVE):
            tmp.append(audios[idx])
            idx += 1
        merged.append(np.concatenate(tmp))
    return merged


def read_wav():
    audios = []
    for i in range(NUM_FILES):
        y, ori_sr = librosa.load(f'./data/{i}.wav', duration=TIME, sr=SAMPLE_RATE)
        # 预处理
        b, a = signal.butter(8, STOP_RATE, fs=ori_sr, btype='lowpass')
        y_filted = signal.filtfilt(b, a, y)
        
        # 低通滤波前后的频谱变化
        # plt.subplot(2, 1, 1)
        # plt.specgram(y, Fs=ori_sr, scale_by_freq=True, sides='default')
        # plt.xlabel('Time [sec]')
        # plt.subplot(2, 1, 2)
        # plt.specgram(y_filted, Fs=ori_sr, scale_by_freq=True, sides='default')
        # plt.xlabel('Time [sec]')
        # plt.show()

        audios.append(y_filted)
    return audios


def save_wav(audios: List[np.ndarray]):
    for i, audio in enumerate(audios):
        sf.write(f'output/{i}.wav', audio, samplerate=SAMPLE_RATE)


def plot(audios: List[np.ndarray],
         name_suffix: str = ''):
    names = [s + name_suffix for s in ('audios', 'spec')]
    freqs = np.abs(np.fft.fft(audios))
    for data, name in zip((audios, freqs), names):
        fig = plt.figure()
        for i, array in enumerate(data, 1):
            plt.subplot(2, 2, i)
            plt.plot(array)
        plt.savefig(f'output/{name}.jpg')
        plt.close(fig)


audios: List[np.ndarray] = read_wav()
plot(audios)


def run():
    spectrums = [np.fft.fft(audio) for audio in split_audio(audios)]
    sample_len = SAMPLE_RATE * SPLIT_TIME // 2
    range1 = [sample[:sample_len] for sample in spectrums]
    range2 = [sample[-sample_len:] for sample in reversed(spectrums)]
    f = np.concatenate(range1 + range2)

    y = np.fft.ifft(f).real
    fig = plt.figure()
    plt.subplot(2, 1, 1)
    plt.plot(np.abs(f))
    plt.subplot(2, 1, 2)
    plt.plot(y)
    plt.savefig('output/sampled.jpg')
    plt.close(fig)

    # 频域解码
    f = np.fft.fft(y)
    processed_specs = []
    for i in range(NUM_WAVES):
        spec = np.concatenate((f[(sample_len * i):(sample_len * (i + 1))],
                               f[(sample_len * (2 * NUM_WAVES - 1 - i)):(sample_len * (2 * NUM_WAVES - i))]))
        processed_specs.append(spec)

    # 恢复时域
    processed_audios = []
    for spec in processed_specs:
        processed_audios.append(np.fft.ifft(spec).real)
    processed_audios = merge_audio(processed_audios)

    plot(processed_audios, '_processed')

    mse = np.mean(np.sum((np.array(audios) - np.array(processed_audios))**2, 1))
    print(f'time segment = {SPLIT_TIME}s, mse = {mse}')

    save_wav(processed_audios)


for SPLIT_TIME in [1, 2, 5, 10, 30]:
    NUM_PER_WAVE = TIME // SPLIT_TIME
    NUM_WAVES = 4 * NUM_PER_WAVE
    run()
