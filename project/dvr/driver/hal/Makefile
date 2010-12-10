CFLAGS:=-I../74HC165 -I../adv7441 -I../gpio -I../gpio-i2c -I../hiir -I../mt9d131 -I../rtc \
-I../tlv320aic31 -I../tw2865-8cif -I../wtdg -I../front_board

.PHONY:all
all:
	arm-hismall-linux-gcc $(CFLAGS) ./devhal.c -O2 -fPIC -shared -o libdevhal.so	

.PHONY:clean
clean:


