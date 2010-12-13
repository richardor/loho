#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include"clocks_rtc.h"
#include"public_typedef.h"
#include"public_function.h"
#include"rtc_ds1339.h"

        
typedef struct _PrivInfo
{
	int fd_rtc;	
	rtc_time_t rtc_time;
}PrivInfo;


static int rtc_open(char *rtc_dev_name)
{
	return_val_if_failed(rtc_dev_name != NULL, -1);

	int fd_rtc = -1;

	fd_rtc = open(rtc_dev_name, O_RDWR);	

	return fd_rtc;
}


#define DateTimeConversionRtcTime(date_time, rtc_time) if(date_time){\
														rtc_time.year = date_time->year;\
														rtc_time.month = date_time->month;\
														rtc_time.date = date_time->day;\
														rtc_time.hour = date_time->hour;\
														rtc_time.minute = date_time->minute;\
														rtc_time.second = date_time->second;\
														rtc_time.weekday = date_time->week;\
														}

#define RtcTimeConversionDateTime(rtc_time, date_time) if(date_time){\
														date_time->year = rtc_time.year;\
														date_time->month = rtc_time.month;\
														date_time->day = rtc_time.date;\
														date_time->hour = rtc_time.hour;\
														date_time->minute = rtc_time.minute;\
														date_time->second = rtc_time.second;\
														date_time->week = (Week)rtc_time.weekday;\
														}





static Ret clocks_rtc_set_time(Clocks *thiz, Datetime *date_time)
{	
	return_val_if_failed(thiz != NULL &&date_time != NULL, RET_INVALID_PARAMETER);

	Ret ret = RET_FAILED;
	rtc_time_t rtc_time = {0};
	
	DateTimeConversionRtcTime(date_time, rtc_time)
	
	if(thiz->priv_subclass[0])
	{
		DECL_PRIV0(thiz, priv0);
		if(ioctl(priv0->fd_rtc, RTC_TIME_SET, &rtc_time) == 0)	
		{
			ret = RET_OK;
		}	 	
	}

	return ret;
}


static Ret clocks_rtc_get_time(Clocks *thiz, Datetime *date_time)
{
	return_val_if_failed(thiz != NULL && date_time != NULL, RET_INVALID_PARAMETER);
	
	if(thiz->priv_subclass[0])
	{
		DECL_PRIV0(thiz, priv0);
		RtcTimeConversionDateTime(priv0->rtc_time, date_time)
	}
	return RET_OK;
}

static void clocks_rtc_destroy(Clocks *thiz)
{
	return_if_failed(thiz != NULL);

	if(thiz->priv_subclass[0])
	{
		DECL_PRIV0(thiz, priv0);
		if(priv0->fd_rtc >= 0)
		{
			close(priv0->fd_rtc);
			priv0->fd_rtc = -1;
		}
		COMM_ZFREE(thiz->priv_subclass[0], sizeof(PrivInfo));		
	}	

	return;
}

static void *clocks_rtc_time_thread(void *para)
{
	return_val_if_failed(para != NULL, NULL);	

	Clocks *thiz = (Clocks *)para;
	rtc_time_t rtc_time_current;
	int ret;
	Datetime date_time = {0};
	Datetime *p_date_time = &date_time;

	DECL_PRIV0(thiz, priv0);
	while(1)
	{
		/*read time*/
		DECL_PRIV0(thiz, priv0);
		ret = ioctl(priv0->fd_rtc, RTC_TIME_GET, &rtc_time_current);		
		if(ret < 0)
		{
			printf("Get rtc time Failed! ret(%d)\n", ret);
			goto OUT;
		}
		if(rtc_time_current.second != priv0->rtc_time.second)
		{
			RtcTimeConversionDateTime(rtc_time_current, p_date_time)
			memcpy(&(priv0->rtc_time), &rtc_time_current, sizeof(rtc_time_t));		
		}			
		public_msleep(30);
	}

OUT:
	return NULL;
}

static void clocks_rtc_run(Clocks *thiz)
{
	return_if_failed(thiz != NULL);

	/*create a thread to get rtc time*/
	
	create_normal_thread(clocks_rtc_time_thread, (void *)thiz, NULL);
	
	return;
}

Clocks *clocks_rtc_create(char *rtc_dev_name)
{
	return_val_if_failed(rtc_dev_name != NULL, NULL);
	
	Clocks *clocks = (Clocks *)COMM_ZALLOC(sizeof(Clocks));
	return_val_if_failed(clocks != NULL, NULL);
	int fd_rtc = -1;

	clocks->priv_subclass[0] = (PrivInfo *)COMM_ZALLOC(sizeof(PrivInfo));
	if(clocks->priv_subclass[0] != NULL)
	{
		if((fd_rtc = rtc_open(rtc_dev_name)) < 0)
		{
			return NULL;
		}

		DECL_PRIV0(clocks, priv0);
		priv0->fd_rtc = fd_rtc;
		/*set the function*/
		clocks->get_time = clocks_rtc_get_time;			
		clocks->set_time = clocks_rtc_set_time;
		clocks->destroy = clocks_rtc_destroy;
		/*init the clocks*/
		clocks_init(clocks);
		/*clocks run */
		clocks_rtc_run(clocks);
	}
	
	return clocks;
}



