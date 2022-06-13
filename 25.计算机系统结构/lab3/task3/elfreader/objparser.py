import re
import argparse
import os

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", dest = "filename", type = str, default = "", help = "Name of the elf")
    parser.add_argument("-p", "--prefix", dest = "prefix", type = str, default = "0xaaaaaaaaa", help = "the prefix of virtual address")
    parser.add_argument("--debug", dest = "debug", action = "store_true", help = "Print debug infomation")
    args = parser.parse_args()
    filename = args.filename
    debug = args.debug
    prefix = args.prefix

    if not os.path.isfile(filename):
        print("\"{0}\" is not a file".format(filename))
        return

    if not os.access(filename, os.X_OK):
        print("\"{0}\" is not a excutable file".format(filename))
        return

    try:
        os.system("objdump -d {0} >> {1}.dump".format(filename, os.path.basename(filename)))
        f = open("{0}.dump".format(os.path.basename(filename)))
        content = f.read()
        f.close()

        # print(content)
        init_s = re.search("<_init>:\s+[0-9a-fA-F]+:", content).group().split(":")[1].strip()
        main_s = re.search("<main>:\s+[0-9a-fA-F]+:", content).group().split(":")[1].strip()
        main_r = re.split("[\n:]", re.search("<main>:.+?ret", content, re.DOTALL).group())[-2].strip()
        fini_r = re.split("[\n:]", re.search("<_fini>:.+?ret", content, re.DOTALL).group())[-2].strip()
        if debug:
            print("init_s: " + init_s)
            print("fini_r: " + fini_r)
            print("main_s: " + main_s)
            print("main_r: " + main_r)
        else:
            print(prefix + init_s)
            print(prefix + fini_r)
            print(prefix + main_s)
            print(prefix + main_r)    

        os.system("rm {0}.dump".format(os.path.basename(filename)))
    except:
        print("unknown error")

if __name__ == "__main__":
    main()