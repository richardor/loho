
KERNEL_MAKE := -C ../../../../../release/pub/kbuild-FULL_REL
			
obj-m := gpioi2c.o
gpioi2c-y += gpio_i2c.o

EXTRA_CFLAGS+=-I$(PWD)/../gpio-i2c
 
default:	
	arm-hismall-linux-gcc -g -Wall -o i2c_read i2c_read.c strfunc.c	
	arm-hismall-linux-gcc -g -Wall -o i2c_write i2c_write.c	strfunc.c
	make $(KERNEL_MAKE) M=$(PWD) modules  
clean:
	rm *.o; rm i2c_read; rm i2c_write 
	make $(KERNEL_MAKE) M=$(PWD) clean 

