KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL

obj-m := rtc_ds1339.o

rtc-ds1339-y := rtc_ds1339.o

EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c 

all:
	make $(KERNEL_MAKE) M=$(PWD) modules
clean:
	make $(KERNEL_MAKE) M=$(PWD) clean 




