#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL


obj-m := hi_ir.o
hi_ir-y += hiir.o
EXTRA_CFLAGS+=-I$(PWD)/../hiir
default:
	make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 

