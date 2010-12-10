#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
			
obj-m := adv_7441.o
adv_7441-y += adv7441.o
EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c
default:
	@make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	@make $(KERNEL_MAKE) M=$(PWD) clean 

