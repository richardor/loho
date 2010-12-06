#include"date_time.h"


int main(int argc , char *argv[])
{
	int year = 0;
	int month = 0;
	int day = 0;
	int week = 0;
	

	year = 2010;
	month = 8;
	for(int i = 0; i < 31; i++)
	{
		day = i+1;
		week = get_weekday(year, month, day);
		printf("year(%d), month(%d), day(%d)-------------week(%d)\n", year, month, day, week);		
	}

	return 0;	
}