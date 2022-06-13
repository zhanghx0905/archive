import argparse
import train
import predict
from func import load_pkl


def parse():
    parser = argparse.ArgumentParser(description='a Simple PinYin Input Method Based on N-gram',
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument('--init-char', action='store_true',
                        help='init char table')
    parser.add_argument('--init-pinyin', action='store_true',
                        help='init pinyin to char dict')
    parser.add_argument('--preprocess', action='store_true',
                        help='preprocess text')
    parser.add_argument('--train', action='store_true', help='train model')

    parser.add_argument('--encoding', type=str, default='gbk',
                        help='specify the encoding of input file')
    parser.add_argument('--n', type=int, default=2,
                        help='select n gram model, where 2 <= n <= 4')
    parser.add_argument('--file', type=str, default=None,
                        help='file path for training data')
    parser.add_argument('--dir', type=str, default=None,
                        help='dir path for training data')
    parser.add_argument('--match', type=str, default='.*',
                        help='use regex to match files')
    parser.add_argument('--load-model', type=str,
                        default=None, help='path to load model')
    parser.add_argument('--save-model', type=str,
                        default='../model/model.pkl', help='path to save model')
    parser.add_argument('--input-file', type=str,
                        default=None, help='the path of input file')
    parser.add_argument('--output-file', type=str,
                        default=None, help='the path of output file')
    args = parser.parse_args()
    assert 2 <= args.n <= 4
    return args


def main():
    args = parse()

    if args.init_pinyin:
        train.init_pinyin(args)
        return

    if args.init_char:
        train.init_char_table(args)
        return

    if args.preprocess:
        train.preprocess(args)
        return

    if args.load_model is None:
        model = train.get_defaultdict(0)
    else:
        model = load_pkl(args.load_model)

    if args.train:
        train.train(args, model)
        return

    ios = {}
    if args.input_file:
        ios['input'] = open(args.input_file, 'r', encoding=args.encoding)
    if args.output_file:
        ios['output'] = open(args.output_file, 'w')

    predict.predict(args, model, **ios)
    if ios['output']:
        ios['output'].close()


if __name__ == '__main__':
    main()
