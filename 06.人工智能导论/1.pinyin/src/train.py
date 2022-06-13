import json

from cn2an import transform
from pypinyin import lazy_pinyin
from hpara import *
from func import *


def init_pinyin(args):
    pinyin2char = get_defaultdict(['##'], 1)
    char2pinyin = get_defaultdict('')
    lines = load_file(args.file, args.encoding)
    for line in lines:
        pinyin, *chars = line.strip().split(' ')
        new_chars = []
        for char in chars:
            char2pinyin[char][pinyin] = char + str(len(char2pinyin[char]))
            new_chars.append(char2pinyin[char][pinyin])
        pinyin2char[pinyin] = new_chars
    save_pkl(pinyin2char, pinyin2char_path)
    save_pkl(char2pinyin, char2pinyin_path)


def preprocess(args):
    char_exist = load_pkl(char_table_path)
    char2pinyin = load_pkl(char2pinyin_path)
    # convert

    def preprocess_text(line):
        data = json.loads(line)
        text = data['html']
        # text = data['content']
        text = ''.join(
            char if char_exist[char] or char.encode('UTF-8').isdigit() else ' '
            if char in pausing_punc else ''
            for char in text
        )
        sentences = text.split(' ')
        new_sentences = []
        for sentence in sentences:
            try:
                sentence = transform(sentence, 'an2cn')
            except ValueError:
                pass
            pinyins = lazy_pinyin(sentence, errors='ignore')
            sentence = ''.join(
                char2pinyin[char][pinyin]
                for char, pinyin in zip(sentence, pinyins)
            )
            sentence = re.sub('\s+', '', sentence)
            new_sentences.append(sentence)
        text = "##".join(new_sentences)
        text = f'##{text}##'
        text = re.sub(r'(##){2,}', r'##', text)
        return text

    def do_preprocess(path):
        lines = load_file(path, args.encoding)
        data = []
        for idx, line in enumerate(lines):
            data.append(preprocess_text(line)+'\n')
            print(f'\r{idx+1} lines preprocessed', end='')
        with open(path, 'w') as f:
            f.writelines(data)
        print(f'{path} Finished.')

    files = get_file(args, 'preprocessing')
    for f in files:
        do_preprocess(f)


def get_key(key):
    if key[-2:] == '##':
        return key[-2:]
    return key.split('##')[-1]


def train(args, model):
    char_exist = load_pkl(char_table_path)

    def do_train(path):
        def record(prefix, char):
            model[prefix][char] += 1
            if len(prefix) > 2:
                model[prefix]['tot'] += 1
                record(prefix[2:], char)

        with open(path, 'r', encoding=args.encoding) as f:
            for idx, line in enumerate(f):
                line = line.strip('\n')
                length = len(line)
                model['all']['tot'] += length // 2
                chars = (
                    line[c:(c+2)]
                    for c in range(0, length, 2)
                )
                for char in chars:
                    model[char]['tot'] += 1
                len_grams, len_gram = length - 2 * args.n + 2, 2 * args.n
                grams = (
                    line[c:(c+len_gram)]
                    for c in range(0, len_grams, 2)
                )
                for gram in grams:
                    prefix, char = get_key(gram[:-2]), gram[-2:]
                    record(prefix, char)
                print(f'\r{idx+1} lines trained', end='')
        print(f'{path} Finished')
        return

    files = get_file(args, 'training')
    for f in files:
        do_train(f)
    save_pkl(model, args.save_model)
