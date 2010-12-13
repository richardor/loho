#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"clocks_rtc.h"

#define RTC_DEV_NAME	"/dev/rtc_ds1339"

int main(int argc, char *argv[])
{
	printf("clocks_rtc test begin!\n");
	Clocks *clocks = NULL;
	Datetime date_time = {0};
	
	clocks = clocks_rtc_create(RTC_DEV_NAME);

	while(1)
	{
		sleep(1);	
		clocks_get_time(clocks, &date_time);	
		printf("Get date and time for the clocks [year(%d), month(%d) week(%d), hour(%d), minute(%d), second(%d)]\n",date_time.year,\
				date_time.month,date_time.week,date_time.hour, date_time.minute, date_time.second);
		
	}

	printf("clocks_rtc test over!\n");
	return 0;
}
