obj-m += task2.o
task2-y := debugger.o debug_base.o debug_control.o

PWD := $(shell pwd)
KDIR := /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KDIR) M=$(PWD) modules
	rm *.o .*.cmd 

clean:
	make -C $(KDIR) M=$(PWD) clean

