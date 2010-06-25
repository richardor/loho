/*************************************************
  Copyright (C), 2007-2099,SZ-STREAMING Tech. Co., Ltd.
  File name:
      rmcommondefine.h
  Author:
      杨学政
  Date:
  Description:
      该文件定义一些通用的宏定义
*************************************************/
#ifndef __RMCOMMONDEFINE_H__
#define __RMCOMMONDEFINE_H__

#include "stdio.h"



#define GUI_ASSERT(exp)        assert((exp)?1:(printf ("ASSERT failed: line %d, function %s, file %s\n",\
                                        __LINE__, __FUNCTION__, __FILE__), 0))

#define MT_ASSERT(exp)            assert((exp)?1:(printf ("ASSERT failed: line %d, function %s, file %s\n",\
                                        __LINE__, __FUNCTION__, __FILE__), 0))


/***********************************************************
    分配内存失败后，退出应用程序
************************************************************/
#define MALLOC_FAILED_EXIT(pointer) do{\
    if(pointer == NULL)\
    {\
        exit(1);\
    }\
}while(0);


/***********************************************************
    判断年月日的合法性，注意年月日时分秒都是unsigned char
    类型的
************************************************************/
#define VALID_YEAR(year) (year <= 99)
#define VALID_MONTH(month) ((month >= 1) && (month <= 12))
#define VALID_DAY(day) ((day >= 1) && (day <= 31))
#define VALID_HOUR(hour) (hour <= 23)
#define VALID_MINUTE(minute) (minute <= 59)
#define VALID_SECOND(second) (second <= 59)

/***********************************************************
    判断是否是数字
************************************************************/
#define ISNUMERIC(character) ((character >= '0') && (character <= '9'))


/***********************************************************
    4字节对齐
************************************************************/
#define ASSIGN_FOUR_BYTES(number) (((number) + 3) / 4 * 4)


/***********************************************************
    HI3511 API函数调用的返回值
 ************************************************************/
#define SUCCESS          0
#define FAILURE          (-1)

#define MAX_PATH_LEN    128


/*************************************************************
	P制与N制屏幕的宽度和高度
*************************************************************/
#define SCREEN_WIDTH  		720
#define PAL_SCREEN_HEIGHT   576
#define NTSC_SCREEN_HEIGHT 480



#endif/*__RMCOMMONDEFINE_H__*/


