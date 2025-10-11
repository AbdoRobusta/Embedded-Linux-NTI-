obj-m := gpiomod.o

# Yocto will pass KERNEL_SRC or KERNELDIR automatically
 KDIR ?= $(KERNEL_SRC)
 PWD := $(shell pwd)

 all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean


