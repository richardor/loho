#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"clocks_rtc.h"

#define RTC_DEV_NAME	"/dev/rtc_ds1339"

int main(int argc, char *argv[])
{
	printf("clocks_rtc test begin!\n");

	Clocks *clocks = NULL;
	
	clocks = clocks_rtc_create(RTC_DEV_NAME);



	printf("clocks_rtc test over!\n");

	return 0;
}
