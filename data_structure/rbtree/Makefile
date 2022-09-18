KERNEL_PATH:=/usr/src/linux-headers-$(shell uname -r)/

obj-m:=rbtree_test.o

all:
	$(MAKE) -C $(KERNEL_PATH) M=$(PWD) modules;

clean:
	$(MAKE) -C $(KERNEL_PATH) M=$(PWD) clean;
	rm -rf *.o *.ko
