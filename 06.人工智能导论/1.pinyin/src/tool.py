import argparse


def parse():
    parser = argparse.ArgumentParser(
        description='a Tool to Check the Accuracy of Predictions')

    parser.add_argument('file1', type=str, default=None, help="predict result")
    parser.add_argument('file2', type=str, default=None, help="correct answer")
    parser.add_argument('--encoding1', type=str, default='gbk')
    parser.add_argument('--encoding2', type=str, default='gbk')
    args = parser.parse_args()
    return args


def compare(args, file1, file2):
    f1 = open(file1, encoding=args.encoding1)
    f2 = open(file2, encoding=args.encoding2)
    match, total = 0, 0
    line_match, line_tot = 0, 0
    for line1, line2 in zip(f1, f2):
        line_tot += 1
        line1 = line1.strip("\n")
        line2 = line2.strip("\n")
        if line1 == line2:
            line_match += 1
        for w1, w2 in zip(line1, line2):
            total += 1
            if w1 == w2:
                match += 1
    print(
        f'words match: {match}, total:{total}, rate: {100*match/total:.2f} %')
    print(
        f'sentences match: {line_match}, total:{line_tot}, rate: {100*line_match/line_tot:.2f} %')
    f1.close()
    f2.close()


def main():
    args = parse()
    compare(args, args.file1, args.file2)


if __name__ == '__main__':
    main()
