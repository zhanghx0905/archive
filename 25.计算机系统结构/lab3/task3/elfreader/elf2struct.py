import struct 
import sys

'''
reference: https://blog.csdn.net/alloc7/article/details/6821895
'''

'''
the struct that contain infomation of elf
'''
elfhdr = {}

'''
verify the file is an elf and parse its header
'''
def verify_elf(filename):
    f = open(filename,'rb')
    elfident = f.read(16)
    magic = [i for i in elfident]
    if( magic[0] != 127 or magic[1]!= ord('E') or magic[2] != ord('L') or magic[3] != ord('F')):
        print("your input file %s not a elf file" %filename)
        return
    else:
        temp = f.read(struct.calcsize('2HI3QI6H'))
        temp = struct.unpack('2HI3QI6H',temp)
        global elfhdr
        elfhdr['magic'] = magic
        elfhdr['e_type']= temp[0]
        elfhdr['e_machine'] = temp[1]
        elfhdr['e_version'] = temp[2]
        elfhdr['e_entry'] = temp[3]
        elfhdr['e_phoff'] = temp[4]
        elfhdr['e_shoff'] = temp[5]
        elfhdr['e_flags'] = temp[6]
        elfhdr['e_ehsize'] = temp[7]
        elfhdr['e_phentsize'] = temp[8]
        elfhdr['e_phnum'] = temp[9]
        elfhdr['e_shentsize'] = temp[10]
        elfhdr['e_shnum'] = temp[11]
        elfhdr['e_shstrndx'] = temp[12]
    f.close()

def get_offva_by_name_in_sections(elffile, name_list):
    verify_elf(elffile)
    sections = []
    code_sections = {}
    global elfhdr
    sec_start = elfhdr['e_shoff']
    sec_size = elfhdr['e_shentsize']
    f = open(elffile,'rb')
    f.seek(sec_start)
    for i in range(0,elfhdr['e_shnum']):
        temp = f.read(sec_size)
        temp = struct.unpack('2I4Q2I2Q',temp)
        sec = {}
        sec['sh_name'] = temp[0]
        sec['sh_type'] = temp[1]
        sec['sh_flags'] = temp[2]
        sec['sh_addr'] = temp[3]
        sec['sh_offset'] = temp[4]
        sec['sh_size'] = temp[5]
        sec['sh_link'] = temp[6]
        sec['sh_info'] = temp[7]
        sec['sh_addralign'] = temp[8]
        sec['sh_entsize'] = temp[9]
        sections.append(sec)
    start = sections[elfhdr['e_shstrndx']]['sh_offset']
    for i in range(0,elfhdr['e_shnum']):
        offset = start + sections[i]['sh_name']
        name = get_name(f,offset)
        # print(name)
        if name in name_list:
            print(name + ":  0x%x (size: 0x%x)" % (sections[i]['sh_offset'], sections[i]['sh_size']))
            code_sections[name] = {}
            code_sections[name]["va"] = sections[i]['sh_offset']
            code_sections[name]["size"] = sections[i]['sh_size']
    f.close()
    return code_sections

def get_name(f,offset):
    name = ''
    f.seek(offset)
    while 1:
        c = f.read(1)
        if c == b'\x00':
            break
        else:
            name += c.decode(encoding="utf-8")
    return name

def get_code(elffile, code_from, byte_length):
    f = open(elffile,'rb')
    f.seek(code_from)
    code = f.read(byte_length)
    # print(len(code))
    return code

def example():
    filename = "../break-point/testing/helper"
    name_list = [".init", ".plt", ".text", ".fini"]
    code_sections = get_offva_by_name_in_sections(filename, name_list)
    get_code(filename, code_sections['.init']["va"], code_sections['.init']["size"])