#ifndef __DATE_TIME_H__
#define __DATE_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/


int get_weekday(int year, int month, int day);
int get_pre_month(int year, int month, int *new_year, int *new_month);




#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif