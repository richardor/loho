/*************************************************
  Copyright (C), 2007-2099,SZ-STREAMING Tech. Co., Ltd.
  File name:
    rmdebug.h
  Author:
      杨学政
  Date:
  Description:
*************************************************/
#ifndef _RMDEBUG_H__
#define _RMDEBUG_H__
#include <stdio.h>
#include <stdlib.h>

#define OUTPUT_KEY_INFO/*关键性的打印信息*/
#define OUTPUT_ADDR_INFO/*打印文件名，函数名，行号信息*/
#define OUTPUT_DEBUG_INFO
#define OUTPUT_OSD_DEBUG
#define OUTPUT_GUI_DEBUG
#define OUTPUT_GUIPARSE_INFO
#define OUTPUT_ENCODER_INFO

//#define PCIV_MSG_DEBUG/*打印PCIV发送消息的打印信息*/


#if defined(OUTPUT_GUIPARSE_INFO)
    #define GUIPAR_INFO(info) do{printf info;}while(0)
#else
    #define GUIPAR_INFO(info) do{}while(0)
#endif

#if defined(OUTPUT_GUI_DEBUG)
    #define GUI_DEBUG(info) do{printf info;}while(0)
#else
    #define GUI_DEBUG(info) do{}while(0)
#endif


#if defined(OUTPUT_OSD_DEBUG)
    #define OSD_DEBUG(info) do{printf info;}while(0)
#else
    #define OSD_DEBUG(info) do{}while(0)
#endif


#if defined(OUTPUT_KEY_INFO)
    #define KEY_INFO(info) do{printf info;}while(0)
#else
    #define KEY_INFO(info) do{}while(0)
#endif

#if defined(OUTPUT_DEBUG_INFO)
    #define DEBUG_INFO(info) do{printf info;fflush(stdout);}while(0)
#else
    #define DEBUG_INFO(info) do{}while(0)
#endif

#if defined(OUTPUT_DEBUG_INFO)
	#define D_PRINTF(info) do{printf("%s, %d", __FILE__, __LINE__);printf info;}while(0)
#else
	#define D_PRINTF(info) do{}while(0)
#endif


#if defined(OUTPUT_ENCODER_INFO)
    #define ENCODER_INFO(info) do{printf info;}while(0)
#else
    #define ENCODER_INFO(info) do{}while(0)
#endif

#ifdef PCIV_MSG_DEBUG
    #define PCIV_DBG_INFO(info) do{printf info;fflush(stdout);}while(0)
#else
    #define PCIV_DBG_INFO(info) do{}while(0)
#endif



#if defined(OUTPUT_ADDR_INFO)
    #define ADDR_INFO do{printf("%s(%s):%d\n", __func__, __FILE__, __LINE__);}while(0)
#else
    #define ADDR_INFO do{}while(0)
#endif

#define FAIL_PRINT(retvalue) \
    do{\
        if(HI_SUCCESS != retvalue)\
        {\
            printf("fail in func:%s ret=%x,line=%d\n",__FUNCTION__,retvalue,__LINE__);\
            fflush(stdout);\
        }\
    }while(0)

#define FAIL_RETURN(retvalue) \
    do{\
        if(HI_SUCCESS != retvalue)\
        {\
            printf("fail in func:%s ret=%x,line=%d\n",__FUNCTION__,retvalue,__LINE__);\
            fflush(stdout);\
            return -1;\
        }\
    }while(0)

#define FAIL_EXIT(retvalue) \
    do{\
        if(HI_SUCCESS != retvalue)\
        {\
            printf("fail in func:%s ret=%x,line=%d\n",__FUNCTION__,retvalue,__LINE__);\
            fflush(stdout);\
            exit(-1);\
        }\
    }while(0)

#endif/* _RMDEBUG_H__ */


