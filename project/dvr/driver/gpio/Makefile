#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
			
obj-m := higpio.o
higpio-y += hi_gpio.o
EXTRA_CFLAGS+=-I$(PWD)/../higpio
default:
	@make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	@make $(KERNEL_MAKE) M=$(PWD) clean 
