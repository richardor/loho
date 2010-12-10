
KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL

obj-m := hiwdt.o
hiwdt-y += hi_wdt.o 

default:
	make $(KERNEL_MAKE) M=$(PWD) modules 
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 

