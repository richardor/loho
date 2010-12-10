#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
			
obj-m := tlv_320aic31.o
tlv_320aic31-y += tlv320aic31.o

EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c

default:
	@make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	@make $(KERNEL_MAKE) M=$(PWD) clean 

