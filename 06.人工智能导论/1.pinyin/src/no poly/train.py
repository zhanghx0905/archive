import json

from cn2an import transform

from hpara import *
from func import *

def init_pinyin(args):
    pinyin2char = get_defaultdict(['#'], 1)
    lines = load_file(args.file, args.encoding)
    for line in lines:
        pinyin, *chars = line.strip().split(' ')
        pinyin2char[pinyin] = chars
    save_pkl(pinyin2char, pinyin2char_path)

def preprocess(args):
    char_exist = load_pkl(char_table_path)

    def preprocess_text(line):
        data = json.loads(line)
        text = data['html']
        # text = data['content']
        text = ''.join(
            char if char_exist[char] or char.encode('UTF-8').isdigit() else '#' 
            if char in pausing_punc else ''
            for char in text
        )
        sentences = text.split("#")
        new_sentences = []
        for sentence in sentences:
            try:
                sentence = transform(sentence, 'an2cn')
            except ValueError:
                pass
            sentence = re.sub('\s*','', sentence)
            new_sentences.append(sentence)
        text = "#".join(new_sentences)
        text = f'#{text}#'
        text = re.sub(r'(#){2,}', r'#', text)
        return text

    def do_preprocess(path):
       lines = load_file(path, args.encoding)
       data = []
       for idx, line in enumerate(lines):
           data.append(preprocess_text(line)+'\n')
           print(f'\r{idx+1} news preprocessed', end='')  
       with open(path, 'w') as f:
           f.writelines(data)
       print(f'{path} Finished')      

    files = get_file(args, 'preprocessing')
    for f in files:
        do_preprocess(f)

def get_key(key):
    if key[-1] == '#':
        return key[-1]
    return key.split('#')[-1]

def train(args, model):
    char_exist = load_pkl(char_table_path)

    def do_train(path):      
        def record(prefix, char):
            model[prefix][char] += 1
            if len(prefix) > 1:
                model[prefix]['tot'] += 1
                record(prefix[1:], char)

        with open(path, 'r', encoding=args.encoding) as f:
            for idx, line in enumerate(f):
                length = len(line)
                model['all']['tot'] += length
                for char in line:
                    model[char]['tot'] += 1
                grams = (
                    line[c:c+args.n]
                    for c in range(length - args.n +1)
                )
                for gram in grams:
                    prefix, word = get_key(gram[:-1]), gram[-1]
                    record(prefix, word)
                print(f'\r{idx+1} news trained', end='')
        print(f'{path} Finished')                  
        return
    
    files = get_file(args, 'training')
    for f in files:
        do_train(f)
    save_pkl(model, args.save_model)