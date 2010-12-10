
KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL

obj-m := tw2865.o
tw_2865-y += tw2865.o

EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c 

default:
	make $(KERNEL_MAKE) M=$(PWD) modules  
clean: 
	make $(KERNEL_MAKE) M=$(PWD) clean 



