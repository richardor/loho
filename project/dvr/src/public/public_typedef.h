#ifndef __PUBLIC_TYPEDEF_H__
#define __PUBLIC_TYPEDEF_H__



#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum _Ret{
	RET_OK,
	RET_FAILED, 
	RET_INVALID_PARAMETER, 
}Ret;


#define return_val_if_failed(p, val) if(!(p))\
	{printf("%s:%d Warning: "#p" failed.\n",\
	__func__, __LINE__); return (val);}

#define return_if_failed(p) if(!(p))\
	{printf("%s:%d Warning: "#p" failed.\n", \
		__func__, __LINE__); return;}




#define DECL_PRIV0(thiz, priv) PrivInfo *priv = (thiz != NULL)?(PrivInfo *)thiz->priv_subclass[0]:NULL





#ifdef __cplusplus
#define DECLS_BEGIN extern "C" {
#define DECLS_END   }
#else
#define DECLS_BEGIN
#define DECLS_END
#endif/*__cplusplus*/



#define COMM_ALLOC(s)       malloc(s)
#define COMM_REALLOC(p, s)  realloc(p, s)
#define COMM_ZALLOC(s)      calloc(1, s)
#define COMM_FREE(p)        if(p) {free(p); p = NULL;}
#define COMM_ZFREE(p, size) if(p) {memset((p), 0x00, (size)); free(p); p = NULL;}




typedef  enum _Week{
    WK_SUNDAY,
    WK_MONDAY,
    WK_TUESDAY,
    WK_WEDNESDAY,
    WK_THURSDAY,
    WK_FRIDAY,
    WK_SATURDAY
}Week;


typedef struct _Datetime{
	int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    Week week;
}Datetime;

typedef enum _LogLevel
{
	LOG_LEVEL_V = 0,
	LOG_LEVEL_D = 1,
	LOG_LEVEL_E = 2,
	LOG_LEVEL_NR,
}LogLevel;

typedef enum _LogSaveMode
{
	LOG_MODE_PRINT = 0, 
	LOG_MODE_FILE = 1,
	LOG_MODE_NR,
}LogSaveMode;



#endif /*__PUBLIC_TYPEDEF_H__*/
