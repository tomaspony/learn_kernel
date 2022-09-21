BASEINCLUDE ?= /home/rlk/rlk/runninglinuxkernel_5.0/
KERN_VERN := $(shell uname -r)

obj-m := second.o

all:
	$(MAKE) -C /usr/src/linux-headers-$(KERN_VERN) M=$(PWD) modules;
clean:
	$(MAKE) -C /usr/src/linux-headers-$(KERN_VERN) M=$(PWD) clean;
	rm -f *.ko





