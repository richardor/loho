#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
			
obj-m := mt_9d131.o
mt_9d131-y += mt9d131.o

EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c

default:
	@make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	@make $(KERNEL_MAKE) M=$(PWD) clean 

