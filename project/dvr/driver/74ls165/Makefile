#
# Makefile for the Linux kernel device drivers.
#
# 2006-04-04, designed by huawei corp.
# written as internal module's Makefile.
#

#KERNEL_MAKE := -C ../../../pub/kbuild-LESS_REL
KERNEL_MAKE := -C ../../../../../../linux-2.6.24
obj-m += 74ls165.o
key_driver-y := 74ls165_gpio_driver.o


default:
	make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 
