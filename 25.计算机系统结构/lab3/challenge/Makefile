obj-m += challenge.o
challenge-y := debugger.o debug_base.o debug_device.o debug_control.o tools.o

PWD := $(shell pwd)
KDIR := /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KDIR) M=$(PWD) modules
	rm *.o .*.cmd 

clean:
	make -C $(KDIR) M=$(PWD) clean

