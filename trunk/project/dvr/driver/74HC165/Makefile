#
# Makefile for the Linux kernel device drivers.
#
# 2006-04-04, designed by huawei corp.
# written as internal module's Makefile.
#

KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL

obj-m += 74hc165.o
drv_74hc165-y := 74hc165.o


default:
	make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 
