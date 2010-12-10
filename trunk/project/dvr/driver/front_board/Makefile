#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
#KERNEL_MAKE := -C ../../../../../../linux-2.6.24

obj-m := key_driver.o
key_driver-y += hi_key_gpio_driver.o

default:
	make $(KERNEL_MAKE) M=$(PWD) modules 
	cp *.ko ../../../release
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 



