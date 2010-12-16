#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"clocks_rtc.h"
#include"print_observer.h"

#define RTC_DEV_NAME	"/dev/rtc_ds1339"

int main(int argc, char *argv[])
{
	printf("clocks_rtc test begin!\n");
	Clocks *clocks = NULL;
	Datetime date_time = {0};
	Observer *print_observer = NULL;

	print_observer = print_observer_create();
	clocks = clocks_rtc_create(RTC_DEV_NAME);
	clocks_add_observer(clocks, print_observer);

	while(getchar() != 'q')
	{
		sleep(3);	
		clocks_remove_observer(clocks, print_observer);
		sleep(3);
		printf("add the observer to the clocks!\n");
		clocks_add_observer(clocks, print_observer);
		clocks_get_time(clocks, &date_time);	
		printf("Get date and time for the clocks [year(%d), month(%d) week(%d), hour(%d), minute(%d), second(%d)]\n",date_time.year,\
				date_time.month,date_time.week,date_time.hour, date_time.minute, date_time.second);
	}
	
	observer_destroy(print_observer);
	clocks_destory(clocks);
	printf("clocks_rtc test over!\n");
	return 0;
}
