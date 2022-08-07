KERN_PATH :=  /home/rlk/rlk/runninglinuxkernel_5.0/

obj-m := test_list.o
all:
	$(MAKE) -C $(KERN_PATH) M=$(PWD) modules;
clean:
	$(MAKE) -C $(KERN_PATH) M=$(PWD) clean;
	rm -f *.ko





