obj-m += task1.o

PWD := $(shell pwd)
KDIR := /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

