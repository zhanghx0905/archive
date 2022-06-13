import heapq
import sys
from math import log
from time import time

from func import load_pkl
from hpara import *
from train import get_key


def predict(args, model, input=sys.stdin, output=sys.stdout):

    pinyin2char = load_pkl(pinyin2char_path)

    def cal_prob(prefix, char):
        '''calculate P(char|prefix)_smooth'''
        def do_calc(prefix, char):
            '''P(char|prefix) = #(prefix, char) / #(prefix)'''
            condition = 0
            if prefix in model:
                condition = model[prefix].get(char, 0) / \
                    (model[prefix]['tot'] + eps)
            return condition
        key_len = 2 * (args.n - 1)
        prefix = get_key(prefix[-key_len:])
        lamba, sum = 1, 0
        for i in reversed(range(2, len(prefix)+1, 2)):
            tmp = prefix[-i:]
            smooth = lamba * model[tmp]['tot']/(gamma + model[tmp]['tot'])
            sum += smooth * do_calc(tmp, char)
            lamba -= smooth
        sum += lamba * model[char]['tot'] / \
            model['all']['tot'] + eps  # avoid underflow
        return sum

    def dp(words):
        '''Viterbi algorithm with beam search, not perfect for n >= 3'''
        def get_max_pair(pair_list):
            return max(pair_list, key=lambda x: x[1])

        def get_topk_pairs(pair_list, k):
            return heapq.nlargest(k, pair_list, key=lambda x: x[1])

        result = [
            (word, log(model[word]['tot'] + eps))
            for word in words[0]
        ]
        for i in range(1, len(words)):
            words_list = words[i]
            rst_tmp = []
            for word in words_list:
                possible_pairs = (
                    (text + word, prob + log(cal_prob(text, word)))
                    for text, prob in result
                )
                if band_width == 1:
                    rst_tmp.append(get_max_pair(possible_pairs))
                else:
                    rst_tmp.extend(get_topk_pairs(possible_pairs, band_width))
            result = rst_tmp
        text, _ = get_max_pair(result)
        return text

    print("Initialization finished.")
    start = time()
    for line in input:

        line = line.strip('\n').strip().lower()
        pinyins = f'## {line} ##'
        pinyin_list = pinyins.split(' ')
        words = [  # a list of possible characters for each pinyin
            pinyin2char[pinyin]
            for pinyin in pinyin_list
        ]
        text = dp(words)
        text = ''.join(
            char for char in text
            if char != '#' and not char.encode('UTF-8').isdigit()
        )
        print(text, file=output)
    end = time()
    print(f"Finished procession in {end - start:.2f} s")
