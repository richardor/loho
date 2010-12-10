#ifndef __RTC_DS1307__
#define __RTC_DS1307__



#define RTC_TIME_GET						0x1
#define RTC_TIME_SET						0x2


typedef struct
{
    unsigned int  second;
    unsigned int  minute;
    unsigned int  hour;
    unsigned int  date;
    unsigned int  weekday;
    unsigned int  month;
    unsigned int  year;
} rtc_time_t;


#endif 
