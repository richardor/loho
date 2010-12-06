#include"date_time.h"
#include<stdio.h>

int main(int argc , char *argv[])
{
	int year = 0;
	int month = 0;
	int day = 0;
	int week = 0;
	int i =0;
	int pre_year = 0;
	int pre_month = 0;
	

	year = 2010;
	month = 8;
	for(i = 0; i < 31; i++)
	{
		day = i+1;
		week = get_weekday(year, month, day);
		printf("year(%d), month(%d), day(%d)-------------week(%d)\n", year, month, day, week);		
	}

	
	get_pre_month(year, month, &pre_year, &pre_month);
	printf("%d- %d pre_year(%d), pre_month(%d)\n",year, month, pre_year, pre_month);

	year = 2001; 
	month = 1;
	get_pre_month(year, month, &pre_year, &pre_month);
	printf("%d- %d pre_year(%d), pre_month(%d)\n",year, month, pre_year, pre_month);


	year = 2001; 
	month = 12;
	get_next_month(year, month, &pre_year, &pre_month);
	printf("%d- %d next_year(%d), next_month(%d)\n",year, month, pre_year, pre_month);


	year = 2037; 
	month = 12;
	get_next_month(year, month, &pre_year, &pre_month);
	printf("%d- %d next_year(%d), next_month(%d)\n",year, month, pre_year, pre_month);


	return 0;	
}
