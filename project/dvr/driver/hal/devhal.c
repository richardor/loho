/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : main.cpp
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/24
  Description   : 
  History       : modify
  1.Date        : 2010/02/03
    Author      : z50825
    Modification: add ir interface,and get serial interface
  2.Date        : 2010/02/10
    Author      : z50825
    Modification: modify to adpt hi3515 4 cif audio
  3.Date		: 2010/2/25
    Author	       : c54122
    Modification: modify ir key value
  4.Date		: 2010/3/04
    Author	       : z50825
    Modification: fix bug of set volume	
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/ioctl.h> 
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <termios.h>
//#include <linux/watchdog.h>

 
#include "devhal.h"
#include "tw2865.h"
#include "rtc_ds1339.h"
#include "watchdog.h"
#include "74hc165.h"
#include "hi_gpio.h"
#include "hiir.h"
#include "hi_key_gpio_driver.h"
#define TW2865_CHIP_NUM 2

#define WDT_DEV         "/dev/watchdog"
#define HI2865_DEV      "/dev/tw2865dev"
#define RTC_DEV         "/dev/rtc_ds1339"

#ifdef NO_USE_74HC165
#define ALARM_IN_DEV   "/dev/hi_gpio"
#else
#define ALARM_IN_DEV	"/dev/74hc165"
#endif
//modify by lyh 
#define PTZ_DEV     "/dev/ttyAMA1"		//modify by lyh 2010.03.01 ttyAMA2-->ttyAMA1
#define BUZZER_DEV  "/dev/hi_gpio"
#define ALARM_OUT_DEV  "/dev/hi_gpio"
#define HDDLED_DEV	"/dev/hi_gpio"
#define KEY_DEV		"/dev/hi_key_gpio"

#define MAX_VIDEO_NUM   16
#define MAX_PROT_CHAR_SIZE  64
#define MAX_PROTOCOL_NUM 16
#define TW2866_DEV		1
// #define TW2865_DEV		1

typedef enum _ptzcmdtable_{

	PCT_STOP,
	PCT_UP,
	PCT_RIGHT_UP,
	PCT_RIGHT,
	PCT_RIGHT_DOWN,
	PCT_DOWN,
	PCT_LEFT_DOWN,
	PCT_LEFT,
	PCT_LEFT_UP,
	
	PCT_ZOOM_INC,
	PCT_ZOOM_DEC,

	PCT_FOCUS_INC,
	PCT_FOCUS_DEC,
	
	
	PCT_IRIS_INC,
	PCT_IRIS_DEC,
	

	PCT_SET_PRESET,
	PCT_CLR_PRESET,
	PCT_CALL_PRESET
	

}ptzcmdtable_t;

typedef enum _ptzprotocol_{
	PP_PELCO_D,
	PP_PELCO_P	
}ptzprotocol_t;

HI_S32 g_s32AudioDaFd = -1;
HI_S32 g_s32AudioAdFd = -1;
HI_S32 g_s32VideoDaFd = -1;
HI_S32 g_s32VideoAdFd = -1;
HI_S32 g_s32RtcFd = -1;
HI_S32 g_s32WdtFd = -1;
HI_S32 g_s32AlarmInFd = -1;
HI_S32 g_S32Irfd = -1;
HI_S32 g_S32Ptzfd = -1;  //add by guixing 091116
HI_S32 g_S32Buzzerfd = -1;  //add by guixing 091117
HI_S32 g_S32AlarmOutfd = -1;  //add by guixing 091117
HI_S32 g_S32KeyFd = -1; //Ziven@100224 for keyboard
HI_S32 g_S32Hddledfd = -1;  //Ziven@100426 for Hdd_Led

pthread_mutex_t g_PtzLock;

/*all the protocols defined by channels,such as Ch1="Pelco-D" , Ch2="Pelco-P" etc*/
HI_U8 g_u8ProtocolChn[MAX_CHN_NUM][MAX_PROT_CHAR_SIZE];

/*the ptz protocol numbers supported by HAL */
HI_U8 g_u8ProtocolNum[MAX_PROTOCOL_NUM][MAX_PROT_CHAR_SIZE];

static HI_S32 s_s32WdtEnable = 0;

HI_S32 HIDVR_HAL_RTC_Open(HI_VOID)
{
	if(g_s32RtcFd < 0)	
	{	    
    	g_s32RtcFd = open(RTC_DEV, O_RDWR);	    
    	if(g_s32RtcFd < 0)	    
    	{	               
        	printf("HIDVR_HAL_RTC_Open Failed!\n");	        
    	    return HI_FAILURE;    
    	}	
	}  
	
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_RTC_Close(HI_VOID)
{
    if(g_s32RtcFd < 0)
	{	   
	    printf("warning: Rtc Closed Already!\n");	
	}
	else
	{
        close( g_s32RtcFd );	   
        g_s32RtcFd = -1;
	}
}

HI_S32 HIDVR_HAL_RTC_SetTime(HAL_RTC_TIME_S* pstTime)
{
	rtc_time_t  stTimeTmp;
	HI_S32 ret = 0;	
	memset(&stTimeTmp, 0, sizeof(rtc_time_t));
	
	if(0 == pstTime)	
	{		
    	printf("Invalid RTC Parameter!\n");		
    	return HI_FAILURE; 	
	}    
	stTimeTmp.second = pstTime->second;    
	stTimeTmp.minute = pstTime->minute;    
	stTimeTmp.hour = pstTime->hour;    
	stTimeTmp.date = pstTime->date;    
	stTimeTmp.weekday = pstTime->weekday;    
	stTimeTmp.month = pstTime->month;    
	stTimeTmp.year = pstTime->year;
	
	ret = ioctl(g_s32RtcFd, RTC_TIME_SET, &stTimeTmp); 	
    if(ret)
	{		
	    printf("Set RTC Time Failed!\n");		
	    return HI_FAILURE;			
	}
	
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_RTC_GetTime(HAL_RTC_TIME_S* pstTime)
{
	HI_S32 ret = 0;	    	
	rtc_time_t stTmp1339; 
    memset(&stTmp1339, 0, sizeof(rtc_time_t));
	
	ret = ioctl(g_s32RtcFd, RTC_TIME_GET, &stTmp1339);    
	if (ret < 0)    
	{        
	    printf("Get RTC Time Failed!\n");		
	    return HI_FAILURE;		 
	}  
	
	memcpy(pstTime, &stTmp1339, sizeof(HAL_RTC_TIME_S)); 
	
	return HI_SUCCESS;
}


HI_S32 HIDVR_HAL_WDT_Open(HI_VOID)
{
	if(g_s32WdtFd < 0)
	{
        g_s32WdtFd = open(WDT_DEV, O_RDWR);
        if(g_s32WdtFd < 0)
        {
            printf("HIDVR_HAL_WDT_Open Failed!\n");
            return HI_FAILURE;
        }
	}

	if(HIDVR_HAL_WDT_Disable())    
	{        
	    printf("When Open Watchdog,Disable Watchdog Failed!\n");        
	    return HI_FAILURE;    
	}

	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_WDT_Close(HI_VOID)
{
	if(HIDVR_HAL_WDT_Disable())	
	{	   
    	printf("Before Close, Disable Failed!\n");	   
    	return HI_FAILURE; 
	}

	if(g_s32WdtFd < 0)
	{	   
	    printf("warning: Watchdog Closed Already!\n");	
	}
	else
	{
        close( g_s32WdtFd );	   
        g_s32WdtFd = -1;
	}
}

HI_S32 HIDVR_HAL_WDT_SetTimeout(HI_U32 u32Msec)
{
	HI_S32 ret = 0;	
	HI_U32 second = u32Msec/1000;
	
	if(s_s32WdtEnable != 1)	
	{		
    	printf("Watchdog Not Enabled!\n");		
    	return HI_FAILURE;  	
	}	
	
	ret = ioctl(g_s32WdtFd, WDIOC_SETTIMEOUT, &second);	
	if(ret)
	{
	    printf("HIDVR_HAL_WDT_SetTimeout Failed!!\n");
        return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_WDT_Enable(HI_VOID)
{
	HI_U32 EnableFlagTmp = WDIOS_ENABLECARD;    
	HI_S32 ret = 0;   
	
	ret = ioctl(g_s32WdtFd, WDIOC_SETOPTIONS, EnableFlagTmp);    
	if(ret == 0)    
	{		
	    s_s32WdtEnable = 1;    
	}    
	else    
	{		
	    printf("HIDVR_HAL_WDT_Enable Failed!\n");		
	    return HI_FAILURE;    
	}    
	
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_WDT_Disable(HI_VOID)
{
	HI_U32 EnableFlagTmp = WDIOS_DISABLECARD;    
	HI_S32 ret = 0;    
	
	ret = ioctl(g_s32WdtFd, WDIOC_SETOPTIONS, EnableFlagTmp);    
	if(ret == 0)    
	{	    
	    s_s32WdtEnable = 0;    
	}    
	else    
	{	    
	    printf("HIDVR_HAL_WDT_Disable Failed!\n");  	    
	    return HI_FAILURE;    
	} 
	
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_WDT_Feed(HI_VOID)
{
	HI_S32 ret = 0;
	
	if(s_s32WdtEnable != 1)    
	{	    
	    printf("Watchdog Not Enabled!\n");	    
	    return HI_FAILURE;    
	}
	
	ret = ioctl(g_s32WdtFd, WDIOC_KEEPALIVE, 0);    
	if(ret)    
	{        
	    printf("HIDVR_HAL_WDT_Feed Failed!\n");  
	    return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_WDT_Reset(HI_VOID)
{
	if(s_s32WdtEnable != 1)        
	{   
	    printf("Watchdog Not Enabled!\n");          
	    return HI_FAILURE;        
	}
	
	HIDVR_HAL_WDT_SetTimeout(1000);        
	sleep(1);   
	
	return HI_SUCCESS;   
}

HI_S32 HIDVR_HAL_VAD_Open(HI_VOID)
{
    if(g_s32VideoAdFd < 0)
    {
        g_s32VideoAdFd = open(HI2865_DEV, O_RDWR);

        if(g_s32VideoAdFd < 0)
        {
            printf("HIDVR_HAL_VAD_Open Failed!\n");
            return HI_FAILURE;
        }
    }
    
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_VAD_Close(HI_VOID)
{
    if(g_s32VideoAdFd < 0)	
    {	   
        printf("warning: VAD closed already!\n");	
    }
    else
    {
        close(g_s32VideoAdFd);
        g_s32VideoAdFd = -1;
    }
}

HI_S32 HIDVR_HAL_VAD_SetTvSystem(HAL_TV_SYSTEM_E enTvsystem)
{
    tw2865_video_norm stVideoNorm;
    HI_S32 i = 0;
    HI_S32 ret = 0;
    
    if(enTvsystem == DVR_TVSYSTEM_PAL)
    {
        for(i = 0; i < TW2865_CHIP_NUM; i++)
        {
            stVideoNorm.chip = i;
            stVideoNorm.mode = TW2865_PAL;
            ret = ioctl(g_s32VideoAdFd, TW2865_SET_VIDEO_NORM, &stVideoNorm);
            if(ret)
            {
                printf("HIDVR_HAL_VAD_SetTvSystem PAL Failed!\n");  
                return HI_FAILURE;
            }    
        }
    }
    else
    {
        for(i = 0; i < TW2865_CHIP_NUM; i++)
        {
            stVideoNorm.chip = i;
            stVideoNorm.mode = TW2865_NTSC;
            ret = ioctl(g_s32VideoAdFd, TW2865_SET_VIDEO_NORM, &stVideoNorm);
            if(ret)
            {
                printf("HIDVR_HAL_VAD_SetTvSystem NTSC Failed!!\n");
                return HI_FAILURE;
            }  
        } 
    }

	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VAD_GetVideoLoss(HI_S32 s32Chn ,HI_BOOL *pbLoss)
{
    tw2865_video_loss stVideoLoss;
    memset(&stVideoLoss, 0, sizeof(tw2865_video_loss));
    HI_S32 ret = 0;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("GetVideoLoss channel num is error!!!\n");
		return HI_FAILURE;
	}

    stVideoLoss.chip = s32Chn / 4;
    stVideoLoss.ch = s32Chn % 4;
    
    ret = ioctl(g_s32VideoAdFd, TW2865_GET_VIDEO_LOSS, &stVideoLoss);
    if(ret)
    {
        printf("HIDVR_HAL_VAD_GetVideoLoss Failed!!\n");
        return HI_FAILURE;
    }

    *pbLoss = stVideoLoss.is_lost;

	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VAD_SetBrightness(HI_S32 s32Chn, HI_U32 u32Brightness)
{
    tw2865_image_adjust stImageAdjust;
    memset(&stImageAdjust, 0, sizeof(tw2865_image_adjust));
    HI_S32 ret = 0;
    HI_U32 tmpBrightness = u32Brightness;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("SetBrightness channel num is error!!!\n");
		return HI_FAILURE;
	}


    stImageAdjust.chip = s32Chn / 4;
    stImageAdjust.chn = s32Chn % 4;
    stImageAdjust.item_sel |= TW2865_SET_BRIGHT;

    if(u32Brightness < 0)
	{
		tmpBrightness = 0;
	}
	if(u32Brightness > 63)
	{
		tmpBrightness = 63;
	}

    stImageAdjust.brightness = tmpBrightness * 4 + 124;//tmpBrightness * 4 - 128

    ret = ioctl(g_s32VideoAdFd, TW2865_SET_IMAGE_ADJUST, &stImageAdjust);
    if(ret)
    {
        printf("HIDVR_HAL_VAD_SetBrightness Failed!!\n");
        return HI_FAILURE;
    }
        
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VAD_SetSaturation(HI_S32 s32Chn, HI_U32 u32Saturation)
{
	tw2865_image_adjust stImageAdjust;
    memset(&stImageAdjust, 0, sizeof(tw2865_image_adjust));
    HI_S32 ret = 0;
    HI_U32 tmpSaturation = u32Saturation;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("SetSaturation channel num is error!!!\n");
		return HI_FAILURE;
	}

    stImageAdjust.chip = s32Chn / 4;
    stImageAdjust.chn = s32Chn % 4;
    stImageAdjust.item_sel |= TW2865_SET_SATURATION;

    if(u32Saturation < 0)
	{
		tmpSaturation = 0;
	}
	if(u32Saturation > 63)
	{
		tmpSaturation = 63;
	}

    stImageAdjust.saturation = tmpSaturation * 4 + 4;//tmpSaturation * 4

    ret = ioctl(g_s32VideoAdFd, TW2865_SET_IMAGE_ADJUST, &stImageAdjust);
    if(ret)
    {
        printf("HIDVR_HAL_VAD_SetSaturation Failed!!\n");
        return HI_FAILURE;
    }
        
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VAD_SetContrast(HI_S32 s32Chn, HI_U32 u32Contrast)
{
	tw2865_image_adjust stImageAdjust;
    memset(&stImageAdjust, 0, sizeof(tw2865_image_adjust));
    HI_S32 ret = 0;
    HI_U32 tmpContrast = u32Contrast;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("SetContrast channel num is error!!!\n");
		return HI_FAILURE;
	}

    stImageAdjust.chip = s32Chn / 4;
    stImageAdjust.chn = s32Chn % 4;
    stImageAdjust.item_sel |= TW2865_SET_CONTRAST;

    if(u32Contrast < 0)
	{
		tmpContrast = 0;
	}
	if(u32Contrast > 63)
	{
		tmpContrast = 63;
	}

    stImageAdjust.contrast = tmpContrast * 4 - 42;//tmpContrast * 4

    ret = ioctl(g_s32VideoAdFd, TW2865_SET_IMAGE_ADJUST, &stImageAdjust);
    if(ret)
    {
        printf("HIDVR_HAL_VAD_SetContrast Failed!!\n");
        return HI_FAILURE;
    }
        
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VAD_SetHuePlus(HI_S32 s32Chn, HI_U32 u32HuePlus)
{
	tw2865_image_adjust stImageAdjust;
    memset(&stImageAdjust, 0, sizeof(tw2865_image_adjust));
    HI_S32 ret = 0;
    HI_U32 tmpHuePlus = u32HuePlus;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("SetHuePlus channel num is error!!!\n");
		return HI_FAILURE;
	}

    stImageAdjust.chip = s32Chn / 4;
    stImageAdjust.chn = s32Chn % 4;
    stImageAdjust.item_sel |= TW2865_SET_HUE;

    if(u32HuePlus < 0)
	{
		tmpHuePlus = 0;
	}
	if(u32HuePlus > 63)
	{
		tmpHuePlus = 63;
	}

    stImageAdjust.hue= tmpHuePlus * 2 - 62;   

    ret = ioctl(g_s32VideoAdFd, TW2865_SET_IMAGE_ADJUST, &stImageAdjust);
    if(ret)
    {
        printf("HIDVR_HAL_VAD_SetHuePlus Failed!!\n");
        return HI_FAILURE;
    }
        
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_VDA_Open(HI_VOID)
{
    if(g_s32VideoDaFd < 0)
    {
        g_s32VideoDaFd = open(HI2865_DEV, O_RDWR);

        if(g_s32VideoDaFd < 0)
        {
            printf("HIDVR_HAL_VDA_Open Failed!\n");
            return HI_FAILURE;
        }
    }
    
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_VDA_Close(HI_VOID)
{
    if(g_s32VideoDaFd < 0)	
    {	   
        printf("warning: VDA closed already!\n");	
    }
    else
    {
        close(g_s32VideoDaFd);
        g_s32VideoDaFd = -1;
    }
}

HI_S32 HIDVR_HAL_VDA_SetTvSystem(HAL_TV_SYSTEM_E enTvsystem)
{
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_AAD_Open(HI_VOID)
{
    if(g_s32AudioAdFd < 0)
    {
    	g_s32AudioAdFd = open(HI2865_DEV, O_RDWR);

        if(g_s32AudioAdFd < 0)
        {
            printf("HIDVR_HAL_AAD_Open Failed!!\n");
            return HI_FAILURE;
        }
    }
    
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_AAD_Close(HI_VOID)
{
    if(g_s32AudioAdFd < 0)	
    {	   
        printf("warning: AAD closed already!\n");	
    }
    else
    {
        close(g_s32AudioAdFd);
        g_s32AudioAdFd = -1;
    }
}

/******************************************************************************
  1.Date        : 2010/02/10
    Author      : z50825
    Modification: modify to adpt hi3515 4 cif audio
******************************************************************************/
HI_S32 HIDVR_HAL_AUDIO_PLAYBACK(HI_S32 s32Chn, HI_S32 s32DevId, HI_BOOL bLoop)
{
    tw2865_audio_playback stAudioPlayback;
    memset(&stAudioPlayback, 0, sizeof(tw2865_audio_playback));
    HI_S32 ret = 0;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("AUDIO_PLAYBACK channel num is error!!!\n");
		return HI_FAILURE;
	}


	
    if(bLoop == HI_TRUE)/*open loopback */
    {
#ifdef TW2866_DEV
		stAudioPlayback.chip = s32Chn /4;
#else
		stAudioPlayback.chip = 0;
#endif // TW2866_DEV
        stAudioPlayback.chn = s32Chn;
 
		ret = ioctl(g_s32AudioDaFd, TW2865_SET_AUDIO_PLAYBACK, &stAudioPlayback);
        if(ret)
        {
            printf("HIDVR_HAL_AUDIO_PLAYBACK VSPOT Failed!!\n");
            return HI_FAILURE;
        }
    }
	else
	{
#ifdef TW2866_DEV
		stAudioPlayback.chip = s32Chn /4;
#else
		stAudioPlayback.chip = 0;
#endif // TW2866_DEV
        stAudioPlayback.chn = TW2865_FIRST_STAGE_CHIP;

        ret = ioctl(g_s32AudioDaFd, TW2865_SET_AUDIO_PLAYBACK, &stAudioPlayback);
        if(ret)
        {
            printf("HIDVR_HAL_AUDIO_PLAYBACK VOUT Failed!!\n");
            return HI_FAILURE;
        }
	}

	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_AAD_Enable_ChnMute(HI_S32 s32Chn)
{
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_AAD_Disable_ChnMute(HI_S32 s32Chn)
{
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_AAD_SetVolum(HI_S32 s32Chn, HI_U32 u32Vol)
{
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_ADA_Open(HI_VOID)
{
    if(g_s32AudioDaFd < 0)
    {
    	g_s32AudioDaFd = open(HI2865_DEV, O_RDWR);

        if(g_s32AudioDaFd < 0)
        {
            printf("HIDVR_HAL_ADA_Open Failed!!\n");
            return HI_FAILURE;
        }
    }
    
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_ADA_Close(HI_VOID)
{
    if(g_s32AudioDaFd < 0)	
    {	   
        printf("warning: ADA closed already!\n");	
    }
    else
    {
        close(g_s32AudioDaFd);
        g_s32AudioDaFd = -1;
    }
}

HI_S32 HIDVR_HAL_ADA_SetVolum(HI_S32 s32Chn, HI_S32 s32DevId, HI_U32 u32Vol)
{
    tw2865_audio_da_volume stAudioDaVolume;
    memset(&stAudioDaVolume, 0, sizeof(tw2865_audio_da_volume));
    HI_S32 ret = 0;

    if(s32Chn < 0 || s32Chn > MAX_VIDEO_NUM)
	{
	    printf("SetVolum channel num is error!!!\n");
		return HI_FAILURE;
	}

	/*vout output from DA 0*/
    stAudioDaVolume.chip = 0;//adudio_2009
    stAudioDaVolume.volume = u32Vol/4;//adudio_2009

    ret = ioctl(g_s32AudioDaFd, TW2865_SET_AUDIO_DA_VOLUME, &stAudioDaVolume);
    if(ret)
    {
        printf("HIDVR_HAL_ADA_SetVolum Failed!!\n");
        return HI_FAILURE;
    }
    
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_ADA_Enable_Mute(HI_S32 s32DevId)
{
    HI_S32 ret = 0;
    tw2865_audio_da_mute stAudioDaMute;
    memset(&stAudioDaMute, 0, sizeof(tw2865_audio_da_mute));

    if(1 == s32DevId)
    {
        stAudioDaMute.chip = 0;
    }
    else
    {
        stAudioDaMute.chip = 1;
    }

    ret = ioctl(g_s32AudioDaFd, TW2865_SET_AUDIO_DA_MUTE, &stAudioDaMute);
    if(ret)
    {
        printf("HIDVR_HAL_ADA_Enable_Mute Failed!!\n");
        return HI_FAILURE;
    }
    
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_ADA_Disable_Mute(HI_VOID)
{   
	return HI_SUCCESS;
}

#ifdef USE_SENSOR_HAL
HI_S32 HIDVR_HAL_INDEV_ALARM_Open(HI_VOID)
{
#ifdef NO_USE_74HC165
	if(g_s32AlarmInFd < 0)	
	{	    
    	g_s32AlarmInFd = open(ALARM_IN_DEV, O_RDWR);	    
    	if (g_s32AlarmInFd < 0)	    
    	{	        
        	printf("/dev/hi_gpio opened failed!\n");	        
        	return HI_FAILURE;	    
    	}	
	} 

	signed int tmp=0;
	gpio_groupbit_info group_dir_info;

        group_dir_info.groupnumber=5;
        group_dir_info.bitnumber=5;
        group_dir_info.value=GPIO_DIR_IN;  /*change mode*/

        tmp=ioctl(g_s32AlarmInFd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO5_5 dir failed!\n");
        	return -1;
        }

		group_dir_info.groupnumber=2;
        group_dir_info.bitnumber=2;
        group_dir_info.value=GPIO_DIR_IN;  /*change mode*/

        tmp=ioctl(g_s32AlarmInFd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO2_2 dir failed!\n");
        	return -1;
        }

		group_dir_info.groupnumber=2;
        group_dir_info.bitnumber=3;
        group_dir_info.value=GPIO_DIR_IN;  /*change mode*/

        tmp=ioctl(g_s32AlarmInFd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO2_3 dir failed!\n");
        	return -1;
        }

		group_dir_info.groupnumber=2;
        group_dir_info.bitnumber=4;
        group_dir_info.value=GPIO_DIR_IN;  /*change mode*/

        tmp=ioctl(g_s32AlarmInFd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO2_4 dir failed!\n");
        	return -1;
        }
#else
	if(g_s32AlarmInFd < 0)	
	{	    
    	g_s32AlarmInFd = open(ALARM_IN_DEV, O_RDONLY);	    
    	if (g_s32AlarmInFd < 0)	    
    	{	        
        	printf("HIDVR_HAL_INDEV_ALARM_Open Failed!\n");	        
        	return HI_FAILURE;	    
    	}	
	} 
#endif
	return HI_SUCCESS;
}

HI_VOID HIDVR_HAL_INDEV_ALARM_Close(HI_VOID)
{
    if(g_s32AlarmInFd < 0)	
    {	   
        printf("warning: Alarm IN Dev Closed Already!\n");	
    }
    else
    {
        close(g_s32AlarmInFd);
        g_s32AlarmInFd = -1;
    }
}

HI_S32 HIDVR_HAL_INDEV_ALARM_GetState(HI_S32 s32Chn, HI_S32* ps32Alarm)
{
#ifdef NO_USE_74HC165
	if(g_s32AlarmInFd<=0)
    {
    	printf("file not opened yet!\n");
    	return -1;
    }
        
    signed int tmp=0;
    gpio_groupbit_info group_dir_info;

	switch(s32Chn)
    {
        case 0:
                group_dir_info.groupnumber=5;
                group_dir_info.bitnumber=5;
                break;

        case 1:
                group_dir_info.groupnumber=2;
                group_dir_info.bitnumber=2;
                break;

        case 2:
                group_dir_info.groupnumber=2;
                group_dir_info.bitnumber=4;
                break;

        case 3:
                group_dir_info.groupnumber=2;
                group_dir_info.bitnumber=3;
                break;

        default:
                printf("Error Alarm chn !!\n");
                return -1;
                break;
    }

	tmp=ioctl(g_s32AlarmInFd, GPIO_READ_BIT, (unsigned long)&group_dir_info);
    if(tmp!=0)
    {
    	printf("Get Alarm State Failed chn:%d!\n",s32Chn);	
    	return -1;
    }
	//printf("+++++ s32Chn = %d   group_dir_info.value = %d +++++\n",s32Chn,group_dir_info.value);
	*ps32Alarm = group_dir_info.value & 0x01;
#else
	HI_S32 ret = 0;  
	HI_U32 alarm = 0;    
	ret = ioctl(g_s32AlarmInFd, _IOC_GET_VALUE,&alarm);    
	if(ret < 0)	
	{		
    	printf("Get Alarm State Failed chn:%d!\n",s32Chn);		
    	return HI_FAILURE;	
	}    
	*ps32Alarm = (alarm >> s32Chn) & 0x01;
#endif
	return HI_SUCCESS;
}

HI_S32 HIDVR_HAL_OUTDEV_ALARM_Open(HI_VOID)
{
        if(g_S32AlarmOutfd<=0)
        {
        	g_S32AlarmOutfd=open(ALARM_OUT_DEV, O_RDWR);
        	if(g_S32AlarmOutfd<=0)
        	{
        	    printf("/dev/hi_gpio opened failed!\n");
        	    return -1;
        	}
                printf("open /dev/hi_gpio opened ok!\n");
        }

        signed int tmp=0;
        gpio_groupbit_info group_dir_info;

	//modify by lyh 2010.03.08
        group_dir_info.groupnumber=2;
        group_dir_info.bitnumber=1;
        group_dir_info.value=GPIO_DIR_OUT;  /*change out mode of GPIO3_6*/

        tmp=ioctl(g_S32AlarmOutfd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO2_1 dir failed!\n");
        	return -1;
        }
#if 0
        group_dir_info.groupnumber=3;
        group_dir_info.bitnumber=7;
        group_dir_info.value=GPIO_DIR_OUT;  /*change out mode of GPIO3_7*/
        tmp=ioctl(g_S32AlarmOutfd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO3_7 dir failed!\n");
        	return -1;
        }

        group_dir_info.groupnumber=7;
        group_dir_info.bitnumber=0;
        group_dir_info.value=GPIO_DIR_OUT;  /*change out mode of GPIO7_0*/
        tmp=ioctl(g_S32AlarmOutfd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO7_0 dir failed!\n");
        	return -1;
        }

        group_dir_info.groupnumber=7;
        group_dir_info.bitnumber=1;
        group_dir_info.value=GPIO_DIR_OUT;   /*change out mode of GPIO7_1*/
        tmp=ioctl(g_S32AlarmOutfd, GPIO_SET_DIR, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set GPIO7_1 dir failed!\n");
        	return -1;
        }
#endif

	return 0;
}

HI_VOID HIDVR_HAL_OUTDEV_ALARM_Close(HI_VOID)
{
        if(g_S32AlarmOutfd <= 0)
        {
    	    printf("file closed already!\n");
        }

        close(g_S32AlarmOutfd);
        g_S32AlarmOutfd = -1;
}

HI_S32 HIDVR_HAL_OUTDEV_ALARM_OutData(HI_S32 s32Chn, HI_S32 s32Alarm)
{
        if(g_S32AlarmOutfd<=0)
        {
        	printf("file not opened yet!\n");
        	return -1;
        }
        
        signed int tmp=0;
        gpio_groupbit_info group_dir_info;

        switch(s32Chn)
        {
            case 0:
			//modify by lyh 2010.03.08
                    group_dir_info.groupnumber=2;
                    group_dir_info.bitnumber=1;
                    break;
/*
            case 1:
                    group_dir_info.groupnumber=3;
                    group_dir_info.bitnumber=7;
                    break;

            case 2:
                    group_dir_info.groupnumber=7;
                    group_dir_info.bitnumber=0;
                    break;

            case 3:
                    group_dir_info.groupnumber=7;
                    group_dir_info.bitnumber=1;
                    break;
*/
            default:
                    printf("Error Alarm chn !!\n");
                    return -1;
                    break;
        }
        if(s32Alarm == 0)
        {
            group_dir_info.value = 0;
        }
        else
        {
            group_dir_info.value = 1;
        }
        tmp=ioctl(g_S32AlarmOutfd, GPIO_WRITE_BIT, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set bit failed!\n");
        	return -1;
        }
	return 0;
}
#endif

HI_S32 HIDVR_HAL_HDDLED_Open(HI_VOID)
{	
	if(g_S32Hddledfd<=0)
	{
		g_S32Hddledfd=open(HDDLED_DEV, O_RDWR);
		if(g_S32Hddledfd<=0)
		{
			printf("/dev/hi_gpio opened failed!\n");
			return -1;
		}
		printf("open /dev/hi_gpio opened ok!\n");
	}
	/*initialize gpio8*/
	signed int tmp=0;
	gpio_groupbit_info group_dir_info;
	group_dir_info.groupnumber=3;
	group_dir_info.bitnumber=6;	
	group_dir_info.value=GPIO_DIR_OUT;       /*change out mode of GPIO3_1*/
	tmp=ioctl(g_S32Hddledfd,GPIO_SET_DIR,(unsigned long)&group_dir_info);
	if(tmp!=0)
	{
		printf("set dir failed!\n");
		return -1;
	}
	return 0;
}

HI_VOID HIDVR_HAL_HDDLED_Close(HI_VOID)
{
	if(g_S32Hddledfd <= 0)
	{
		printf("file closed already!\n");
	}
	close(g_S32Hddledfd);
	g_S32Hddledfd = -1;
}

HI_S32 HIDVR_HAL_HDDLED_Enable(HI_VOID)
{
	signed int tmp=0;
	gpio_groupbit_info group_dir_info;
	group_dir_info.groupnumber=3;
	group_dir_info.bitnumber=6;
	group_dir_info.value=1;
	if(g_S32Hddledfd<=0)
	{
		printf("file not opened yet!\n");
		return -1;
	}
	tmp=ioctl(g_S32Hddledfd, GPIO_WRITE_BIT, (unsigned long)&group_dir_info);
	if(tmp!=0)
	{
		printf("set bit failed!\n");
		return -1;
	}
	return 0;
}

HI_S32 HIDVR_HAL_HDDLED_Disable(HI_VOID)
{
	signed int tmp=0;
	gpio_groupbit_info group_dir_info;
	group_dir_info.groupnumber=3;
	group_dir_info.bitnumber=6;
	group_dir_info.value=0;
	if(g_S32Hddledfd<=0)
	{
		printf("file not opened yet!\n");
		return -1;
	}
	tmp=ioctl(g_S32Hddledfd, GPIO_WRITE_BIT, (unsigned long)&group_dir_info);
	if(tmp!=0)
	{
		printf("set bit failed!\n");
		return -1;
	}
	return 0;
}

HI_S32 HIDVR_HAL_BUZZER_Open(HI_VOID)
{	
        if(g_S32Buzzerfd<=0)
        {
        	g_S32Buzzerfd=open(BUZZER_DEV, O_RDWR);
        	if(g_S32Buzzerfd<=0)
        	{
        	    printf("/dev/hi_gpio opened failed!\n");
        	    return -1;
        	}
                printf("open /dev/hi_gpio opened ok!\n");
        }

        /*initialize gpio8*/
        signed int tmp=0;
        gpio_groupbit_info group_dir_info;

        group_dir_info.groupnumber=3;
        group_dir_info.bitnumber=7;	//lyh 1-->7
        group_dir_info.value=GPIO_DIR_OUT;       /*change out mode of GPIO3_1*/

        tmp=ioctl(g_S32Buzzerfd,GPIO_SET_DIR,(unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set dir failed!\n");
        	return -1;
        }
        
	return 0;
}

HI_VOID HIDVR_HAL_BUZZER_Close(HI_VOID)
{
        if(g_S32Buzzerfd <= 0)
        {
    	    printf("file closed already!\n");
        }

        close(g_S32Buzzerfd);
        g_S32Buzzerfd = -1;
}

HI_S32 HIDVR_HAL_BUZZER_Enable(HI_VOID)
{
        signed int tmp=0;
        gpio_groupbit_info group_dir_info;
        group_dir_info.groupnumber=3;
        group_dir_info.bitnumber=7;//lyh 1-->7
        group_dir_info.value=1;//0-->1	[xys--2010/3/20]
        if(g_S32Buzzerfd<=0)
        {
        	printf("file not opened yet!\n");
        	return -1;
        }

        tmp=ioctl(g_S32Buzzerfd, GPIO_WRITE_BIT, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set bit failed!\n");
        	return -1;
        }

	return 0;
}

HI_S32 HIDVR_HAL_BUZZER_Disable(HI_VOID)
{
        signed int tmp=0;
        gpio_groupbit_info group_dir_info;

        group_dir_info.groupnumber=3;
        group_dir_info.bitnumber=7;//lyh 1-->7
        group_dir_info.value=0;

        if(g_S32Buzzerfd<=0)
        {
        	printf("file not opened yet!\n");
        	return -1;
        }

        tmp=ioctl(g_S32Buzzerfd, GPIO_WRITE_BIT, (unsigned long)&group_dir_info);
        if(tmp!=0)
        {
        	printf("set bit failed!\n");
        	return -1;
        }

	return 0;
}

static int g_repkey_enable =0;
HI_S32 HIDVR_HAL_KEYBOARD_Open(HI_VOID)
{
	g_S32KeyFd = open(KEY_DEV,O_RDWR);
	if (g_S32KeyFd <0)
	{
	//	printf("HIDVR_HAL_KEYBOARD_Open Error!!\n");
		return -1;
	}
	return 0;
}

HI_VOID HIDVR_HAL_KEYBOARD_Close(HI_VOID)
{
	if(g_S32KeyFd > 0 )
	{
		close(g_S32KeyFd);
		g_S32KeyFd = -1;
	}
}

/*********************************************************************
Function:   
DVR_KEYBOARD_EnableRepKey
Description:   
Calls:		 
Called By:     
parameter:	       
Return:       
author:
l59217
********************************************************************/
int DVR_KEYBOARD_EnableRepKey(int bEnable)
{
	if(g_S32KeyFd ==-1)
	{
	//	DVR_INFO("DVR_KEYBORD file not opened yet,line:%d!\n",__LINE__);
		return -1;
	}
	if (ioctl(g_S32KeyFd, KEY_IOC_SET_ENABLE_REPKEY, (unsigned long *)bEnable) != 0) 
	{
	//	DVR_INFO("set enable repkey error!\n");
		return -1;          
	}
	g_repkey_enable=1;
	return 0;	    
}

keyvaluetable_t HIDVR_HAL_KEYBOARD_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate)
{
	keyvaluetable_t tmp;

	if(guistate == GUI_VIEW)
	{
		tmp = key;
	}
	else if(guistate == GUI_PLAYBACK)
	{
		tmp = key;
	}
	else
	{
		tmp = key;
	}

	return tmp;
		
}

/*********************************************************************
Function:   
DVR_KEYBOARD_SetRepTimeout
Description:   
Calls:		 
Called By:     
parameter:	       
Return:       
author:
l59217
********************************************************************/
int DVR_KEYBOARD_SetRepTimeout(unsigned int msec)
{
	unsigned long reptimeout = msec;
	if(g_S32KeyFd == -1)
	{
	//	DVR_INFO("DVR_KEYBORD file not opened yet,line:%d!\n",__LINE__);
		return -1;
	}
	if(g_repkey_enable == 1)
	{
		if (ioctl(g_S32KeyFd, KEY_IOC_SET_REPKEY_TIMEOUT, (unsigned long *)reptimeout) != 0) 
		{
	//		DVR_INFO("set repkey timeout error !\n");
			return -1;          
		}
	}
	return 0;	
}

keyvaluetable_t HIDVR_HAL_KEYBOARD_ReadKeyVal(HI_VOID)
{
	unsigned char keyvalue = 0xff;
	if(g_S32KeyFd < 0)
	{
	//	DVR_INFO("DVR_KEYBORD file not opened yet,line:%d!\n",__LINE__);
		return KVT_INVALID;
	}
	if (ioctl(g_S32KeyFd, KEY_IOC_GET_KEYVALUE, &keyvalue) != 0) 
	{
	//	DVR_INFO("set enable repkey error!\n");
		return KVT_INVALID;          
	}
	switch(keyvalue)
	{
		case 0:
			return KVT_ONE;
			break;
		case 2:
		//	return KVT_TWO;
			return KVT_CHANNEL_DEC;
			break;
		case 1:
			return KVT_CHANNEL_ADD; 
		//  return KVT_THREE;
			break;
		case 3:
			return KVT_FOUR;
			break;
		case 4:
			return KVT_MULTIPIC;
			break;
		case 5:
			return KVT_FASTBACK;
			break;
		case 6:
			return KVT_PLAY;
			break;
		case 7:
			return KVT_STOP;
			break;
		case 8:
			return KVT_UP;
			break;
		case 9:
			return KVT_DOWN;
			break;
		case 10:
			return KVT_LEFT;
			break;
		case 11:
			return KVT_RIGHT;
			break;
		case 12:
			return KVT_ENTER;
			break;
		case 13:
			return KVT_FASTPLAY;
			break;
		case 14:
			return KVT_SETUP;
			break;
		case 15:
			return KVT_STARTRECORD;
			break;
		case 16:
				return KVT_RESET;
			break;
		case 32:
			return KVT_VGASEL;
			break;
		case 17:
		case 18:
		case 19:
		case 20:				
		case 21:			
		case 22:			
		case 23:			
		case 0xff:
		default:
			return KVT_INVALID;
			break;
	}	
	return KVT_INVALID;
}

HI_S32 HIDVR_HAL_IR_Open(HI_VOID)
{
    g_S32Irfd = open("/dev/Hi_IR", O_RDWR);
    if(g_S32Irfd <= 0)
    {
        printf("InitRemoter open failed\n");
        return -1;
    }

	return 0;
}

HI_VOID HIDVR_HAL_IR_Close(HI_VOID)
{
	if(g_S32Irfd > 0 )
	{
		close(g_S32Irfd);
		g_S32Irfd = -1;
	}
}

keyvaluetable_t HIDVR_HAL_IR_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate)
{
	keyvaluetable_t tmp;

	if(guistate == GUI_VIEW)
	{
		tmp = key;
	}
	else if(guistate == GUI_PLAYBACK)
	{
		tmp = key;
	}
	else
	{
		tmp = key;
	}

	return tmp;
		
}

keyvaluetable_t HIDVR_HAL_IR_ReadKeyVal(HI_VOID)
{
    irkey_info_s tmp;
	keyvaluetable_t RemoteKey = KVT_INVALID;
    if(read(g_S32Irfd, &tmp, sizeof(tmp)) == sizeof(tmp))
    {
		printf("ReadRemoteBody:%08lx, %08lx(%ld)\n",
		           tmp.irkey_datah, tmp.irkey_datal, tmp.irkey_state_code);

        if(tmp.irkey_state_code == 0)
        {
            switch(tmp.irkey_datah)
            {
                case 0x00:
                switch(tmp.irkey_datal)
				{
				    case 0xeb14fe00:
				        RemoteKey = KVT_ONE;
				        break;
				    case 0xe817fe00:
				        RemoteKey = KVT_THREE;
				        break;
				    case 0xe718fe00:
				        RemoteKey = KVT_FOUR;
				        break;
				    case 0xea15fe00:
				        RemoteKey = KVT_TWO;
				        break;
				    case 0xe619fe00:
				        RemoteKey = KVT_MULTIPIC;
				        break;
				    case 0xfa05ff00:
				        RemoteKey = KVT_SIX;
				        break;
				    case 0xf906ff00:
				        RemoteKey = KVT_SEVEN;
				        break;
				    case 0xf807ff00:
				        RemoteKey = KVT_EIGHT;
				        break;
				    case 0xf708ff00:
				        RemoteKey = KVT_NINE;
				        break;
				    case 0xf40bff00:
						RemoteKey = KVT_TEN; //modify by aragon(ou) 20090526
				        break;

					//change disp mode
					case 0xe31cff00:
						RemoteKey = KVT_4X4;
						break;
					case 0xe01fff00:
						RemoteKey = KVT_3X3;
						break;
					case 0xe11eff00:
						RemoteKey = KVT_2X2;
						break;
					case 0xe21dff00:
						RemoteKey = KVT_AUTO;
						break;
					case 0xf609ff00:
						RemoteKey = KVT_VM;
						break;
					case 0xf50aff00:
						RemoteKey = KVT_MULTIPIC;
						break;

					//direction
					case 0xf00ffe00:
						RemoteKey = KVT_ENTER;
						break;
					case 0xf50afe00:
						RemoteKey = KVT_UP;
						break;
					case 0xec13fe00:
						RemoteKey = KVT_DOWN;
						break;
					case 0xe01ffe00:
						RemoteKey = KVT_RIGHT;
						break;
					case 0xf30cfe00:
						RemoteKey = KVT_LEFT;
						break;

					//misc
					case 0xf807fe00:
						RemoteKey = KVT_STARTRECORD;
						break;
					case 0xf20dff00:
						RemoteKey = KVT_SEARCH;
						break;
					case 0xfb04fE00:
						RemoteKey = KVT_SETUP;
						break;
					case 0xe51aff00:
						RemoteKey = KVT_PIP;
						break;
					case 0xe41bff00:
						RemoteKey = KVT_MUTE;
						break;

					//play
					case 0xfc03fe00:
						RemoteKey = KVT_FASTPLAY;
						break;
					case 0xff00fe00:
						RemoteKey = KVT_FASTBACK;
						break;
					case 0xfe01fe00:
						RemoteKey = KVT_PLAY;
						break;
					case 0xfd02fe00:
						RemoteKey = KVT_STOP;
						break;
// 					case 0xbd42ff00:
// 						RemoteKey = KVT_PAUSE;
// 						break;
					case 0xbc43ff00:
						RemoteKey = KVT_SLOWPLAY;
						break;

					//ptz
					case 0xb748ff00:
						RemoteKey = KVT_ZOOMINC;
						break;
					case 0xb34cff00:
						RemoteKey = KVT_ZOOMDEC;
						break;
					case 0xb649ff00:
						RemoteKey = KVT_FOCUSINC;
						break;
					case 0xb24dff00:
						RemoteKey = KVT_FOCUSDEC;
						break;
					case 0xb54aff00:
						RemoteKey = KVT_IRISINC;
						break;
					case 0xb14eff00:
						RemoteKey = KVT_IRISDEC;
						break;
					case 0xb44bff00:
						RemoteKey = KVT_PTZ;
						break;
					case 0xb04fff00:
						RemoteKey = KVT_LOCK;
						break;
					default :
						RemoteKey = KVT_INVALID;
					break;
				}
                break;
                default:
                        RemoteKey = KVT_INVALID;
                    break;
            }
        }
    }
	return RemoteKey;
}


 
HI_S32 HIDVR_HAL_485KEYBOARD_Open(HI_VOID)
{
	return 0;
}

HI_VOID HIDVR_HAL_485KEYBOARD_Close(HI_VOID)
{

}

keyvaluetable_t HIDVR_HAL_485KEYBOARD_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate)
{
	keyvaluetable_t tmp;

	if(guistate == GUI_VIEW)
	{
		tmp = key;
	}
	else if(guistate == GUI_PLAYBACK)
	{
		tmp = key;
	}
	else
	{
		tmp = key;
	}

	return tmp;
		
}

keyvaluetable_t HIDVR_HAL_485KEYBOARD_ReadKeyVal(HI_VOID)
{
	return KVT_INVALID;
}
#ifdef USE_PTZ_HAL
HI_S32 HIDVR_HAL_PTZ_Open(ptzserialconfig_t *pstPara)
{
        pthread_mutex_init(&g_PtzLock, NULL);

        pthread_mutex_lock(&g_PtzLock);
        g_S32Ptzfd = open(PTZ_DEV, O_RDWR);
	if(g_S32Ptzfd < 0)
	{
		printf("OpenPtzSerial open failed(%s)\n", PTZ_DEV);
                pthread_mutex_unlock(&g_PtzLock);
		return -1;
	}
	else
	{
		printf("OpenPtzSerial open success(%s) !\n", PTZ_DEV);
	}
        pthread_mutex_unlock(&g_PtzLock);
        
        HI_S32 i;
        for(i=0; i<MAX_PROTOCOL_NUM; ++i)
        {
            strcpy(g_u8ProtocolNum[i], "");
        }
        strcpy(g_u8ProtocolNum[0], PTZ_PROTOCOL_D);
        strcpy(g_u8ProtocolNum[1], PTZ_PROTOCOL_P);

	return 0;
}

HI_VOID HIDVR_HAL_PTZ_Close(HI_VOID)
{
        pthread_mutex_lock(&g_PtzLock);
	if(g_S32Ptzfd >= 0)
	{
		close(g_S32Ptzfd);
		g_S32Ptzfd = -1;
	}
        pthread_mutex_unlock(&g_PtzLock);
}

HI_S32 HIDVR_HAL_PTZ_Cfg(ptzserialconfig_t pstPara)
{
        if(g_S32Ptzfd < 0)
	{
		printf("OpenPtzSerial open failed(%s)\n", PTZ_DEV);
		return -1;
	}

        pthread_mutex_lock(&g_PtzLock);
        struct termios   opt;
	speed_t tmp;
	tcgetattr(g_S32Ptzfd, &opt); 
	/*set boundrate*/
	switch(pstPara.baud)
	{
		case 0:
			tmp = B1200;
			break;
		case 1:			
			tmp = B2400;		
			break;
		case 2:
			tmp = B4800;		
			break;
		case 3:
			tmp = B9600;
			break;
		default:
			tmp = B9600;
			break;
	}
	tcflush(g_S32Ptzfd, TCIOFLUSH);     
	cfsetispeed(&opt, tmp);
	cfsetospeed(&opt, tmp);   
	tcsetattr(g_S32Ptzfd, TCSANOW, &opt); 
	/*set data bit*/
	tcflush(g_S32Ptzfd, TCIOFLUSH);   
	tcgetattr(g_S32Ptzfd, &opt); 
	tcflush(g_S32Ptzfd, TCIOFLUSH);
	opt.c_cflag &= ~CSIZE;
	switch (pstPara.databits) 						
	{   
		case 3:
			opt.c_cflag |= CS5; 
			break;
		case 2:
			opt.c_cflag |= CS6; 
			break;
		case 1:		
			opt.c_cflag |= CS7; 
			break;
		case 0:     
			opt.c_cflag |= CS8;
			break;   
		default:    
			fprintf(stderr,"Unsupported stop bits\n");
			break;
	}
	/*set stop bit*/  
	switch (pstPara.stopbits)
	{   
		case 0:    
			opt.c_cflag &= ~CSTOPB;  
			break;  
		case 1:    
			opt.c_cflag |= CSTOPB;  
		   	break;
		default:    
			 fprintf(stderr,"Unsupported stop bits\n"); 
			 break;
	} 
	/*set checksum bit*/
	switch (pstPara.parity) 
	{   
		case 0:
			opt.c_cflag &= ~PARENB;  			 /* Clear parity enable */
			opt.c_iflag &= ~INPCK;     			/* Clear parity checking */ 
			break;  
		case 1:
			opt.c_cflag |= (PARODD | PARENB); 	/*odd*/  
			opt.c_iflag |= INPCK;            		 /* Enable parity checking */ 
			break;  
		case 2:
			opt.c_cflag |= PARENB;     			/* Enable parity */    
			opt.c_cflag &= ~PARODD;   			/* even*/     
			opt.c_iflag |= INPCK;       			/* Enable parity checking */
			break;
		default:   
			fprintf(stderr,"Unsupported parity\n"); 
			break;
	}  
	
	tcflush(g_S32Ptzfd, TCIFLUSH); 
	opt.c_oflag  &= ~OPOST;
	opt.c_cc[VTIME] = 128;
	opt.c_cc[VMIN] = 1;
	cfmakeraw(&opt);
	
	tcsetattr(g_S32Ptzfd, TCSANOW, &opt);
        pthread_mutex_unlock(&g_PtzLock);

        return 0;
}

/*********************************************************************
	Function:   
	Description:   
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:       
  	author:
 ********************************************************************/
 HI_S32 SendCmdBySerial(HI_U8 *pCmdBuffer, HI_S32 len)
{
	if(g_S32Ptzfd < 0)
	{
		return -1;
	}
#if 1
        int i;
	for(i = 0; i < len; i ++)
	{
	        printf("%02x  ", pCmdBuffer[i]);
	}
        printf("\n");
#endif
	if(write(g_S32Ptzfd, pCmdBuffer, len) != len)
	{
	        printf("SendCmdBySerial write failed\n");
		return -1;
	}
	fsync(g_S32Ptzfd);

	return 0;
}



HI_S32 HIDVR_HAL_PTZ_Stop(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Left(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed)
{
      if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];
        
        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x04;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x04;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Right(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];
        
        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x02;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x02;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Up(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8]; 

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x08;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x08;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Down(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x10;
            CmdBuffer[4] = s32Speed;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x10;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = s32Speed;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Iris_Inc(HI_S32 S32Chn, HI_U8 u8Addr)
{
         if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x02;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x04;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
                
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Iris_Dec(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x04;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x08;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Zoom_Inc(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x20;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x20;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Zoom_Dec(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x40;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x40;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Focus_Inc(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x01;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x01;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
       
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Focus_Dec(HI_S32 S32Chn, HI_U8 u8Addr)
{
        if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];

        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x80;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x02;
            CmdBuffer[3] = 0x00;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = 0x00;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Bind_Protocol(HI_S32 S32Chn, HI_U8* pu8Protocol)
{
        if(S32Chn >15 || S32Chn < 0)
        {
            printf("Input Chn number err!\n");
            return -1;
        }

        strcpy(g_u8ProtocolChn[S32Chn], pu8Protocol);
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Enum_Protocol(HI_U8 protocol[16][64],HI_U32 *pProtocolCnt)
{
        int i;
        for(i=0; i < MAX_CHN_NUM; ++i)
        {
            if(strcmp(protocol[i], "") == 0)
            {
                *pProtocolCnt = i+1;
                break;
            }
            strcpy(protocol[i], g_u8ProtocolNum[i]);
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_To_Pos(HI_S32 S32Chn, HI_U8 u8Addr, HI_U32 u32Curpt)
{
    if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];
        
        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x07;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = u32Curpt;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x07;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = u32Curpt;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}

HI_S32 HIDVR_HAL_PTZ_Set_Pos(HI_S32 S32Chn, HI_U8 u8Addr, HI_U32 u32Curpt)
{
    if(g_S32Ptzfd < 0)
	{
		printf("SendCmdToPtz you must open the ptz serial first!\n");
		return -1;
	}

        unsigned char CmdBuffer[8];
        
        if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_D) == 0)
        {
            CmdBuffer[0] = 0xff;
            CmdBuffer[1] = u8Addr;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x03;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = u32Curpt;

            CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];
            return SendCmdBySerial(CmdBuffer, 7);
        }
        else if(strcmp(g_u8ProtocolChn[S32Chn], PTZ_PROTOCOL_P) == 0)
        {
            CmdBuffer[0] = 0xa0;
            CmdBuffer[1] = u8Addr - 1;

            CmdBuffer[2] = 0x00;
            CmdBuffer[3] = 0x03;
            CmdBuffer[4] = 0x00;
            CmdBuffer[5] = u32Curpt;

            CmdBuffer[6] = 0xaf;
            CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
            return SendCmdBySerial(CmdBuffer, 8);
        }
        else
        {
            printf("SendCmdToPtz not support ptz protocol\n");
            return -1;
        }
        
	return 0;
}
#endif

HI_S32 HIDVR_HAL_VGA_SetDev(HAL_VGA_DEV_E enVgaDev)
{
	#define VGA_HDMI_GPIO_GROUP 0x3
	#define VGA_HDMI_GPIO_BIT   0x0
	gpio_groupbit_info group_dir_info;
	
	int s32gpioFd = open("/dev/hi_gpio",O_RDWR);
	if(s32gpioFd <= 0)
	{
        printf("open gpio err\n");	
		return HI_FAILURE;
	}
	
    group_dir_info.groupnumber = VGA_HDMI_GPIO_GROUP;
    group_dir_info.bitnumber   = VGA_HDMI_GPIO_BIT;
    group_dir_info.value       = GPIO_DIR_OUT;

    if(ioctl(s32gpioFd,GPIO_SET_DIR,(unsigned long)&group_dir_info) != 0)
    {
        printf("set direction err\n");	
		return HI_FAILURE;
    }

    group_dir_info.groupnumber= VGA_HDMI_GPIO_GROUP;
    group_dir_info.bitnumber  = VGA_HDMI_GPIO_BIT;
    group_dir_info.value      = enVgaDev;

    if(ioctl(s32gpioFd,GPIO_WRITE_BIT,(unsigned long)&group_dir_info) != 0)
    {
		printf("set bit failed!\n");
		return HI_FAILURE;
    }
	
	close (s32gpioFd);
}

/*********************************************************************
	Function:   
		HIDVR_HAL_SYS_GetSerial
	Description:   
		get machine 32 bit exclusive serial code,
		must be exclusive,and return same value when it is called 
		every time.
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		serial code

 ********************************************************************/
HI_S32 HIDVR_HAL_SYS_GetSerialCode()
{
	return 0;
}

/*********************************************************************
	Function:   
	Description:  
		  Byte1 	  Byte2     Byte3     Byte4    Byte5  Byte6   Byte7 
		SynchByte  Address  Command1  Command 2  Data1  Data2  CheckSum
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:       
 ********************************************************************/
 #ifdef USE_PTZ_HAL
HI_S32 HIDVR_HAL_SendCmdToPtzPelco_d(unsigned char cmd, unsigned char addr,  int speed,void *other)
{
	unsigned char CmdBuffer[7];
	int *tiltSpeed = (int*)other;

	CmdBuffer[0] = 0xff;
	CmdBuffer[1] = addr;
	CmdBuffer[2] = 0x00;
	CmdBuffer[3] = 0x00;
	CmdBuffer[4] = 0x00;
	CmdBuffer[5] = 0x00;
	
	switch(cmd)
	{
		case PCT_STOP:
			break;
		case PCT_UP:
			CmdBuffer[3] = 0x08;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = speed;
			break;
		case PCT_DOWN:
			CmdBuffer[3] = 0x10;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = speed;
			break;
		case PCT_LEFT:
			CmdBuffer[3] = 0x04;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = speed;
			break;
		case PCT_RIGHT:
			CmdBuffer[3] = 0x02;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = speed;
			break;
		case PCT_LEFT_UP:
			CmdBuffer[3] = 0x0c;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = *tiltSpeed;
			break;
		case PCT_RIGHT_UP:
			CmdBuffer[3] = 0x0a;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = *tiltSpeed;
			break;
		case PCT_LEFT_DOWN:
			CmdBuffer[3] = 0x14;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = *tiltSpeed;
			break;
		case PCT_RIGHT_DOWN:
			CmdBuffer[3] = 0x12;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = *tiltSpeed;
			break;
		
		case PCT_IRIS_INC:		
			CmdBuffer[2] = 0x04;
			break;
		case PCT_IRIS_DEC:	
			CmdBuffer[2] = 0x02;
			break;
		case PCT_ZOOM_DEC:
			CmdBuffer[3] = 0x20;
			break;
		case PCT_ZOOM_INC:
			CmdBuffer[3] = 0x40;
			break;
		case PCT_FOCUS_INC:
			CmdBuffer[3] = 0x80;
			break;
		case PCT_FOCUS_DEC:
			CmdBuffer[2] = 0x01;
			break;

		case PCT_SET_PRESET:
			CmdBuffer[3] = 0x03;
			CmdBuffer[5] = speed;
			break;
		case PCT_CLR_PRESET:
			CmdBuffer[3] = 0x05;
			CmdBuffer[5] = speed;
			break;
		case PCT_CALL_PRESET:
			CmdBuffer[3] = 0x07;
			CmdBuffer[5] = speed;
			break;
		default:
			printf(("SendCmdToPtzPelco_d not support cmd\n"));
			return -1;
			break;
	}

	CmdBuffer[6] = CmdBuffer[1] + CmdBuffer[2] + \
					CmdBuffer[3] + CmdBuffer[4] + CmdBuffer[5];

	SendCmdBySerial(CmdBuffer, 7);
	
	return 0;
}


/*********************************************************************

	Function:   
	Description:   
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:       
  	author:
 ********************************************************************/
HI_S32 HIDVR_HAL_SendCmdToPtzPelco_p(unsigned char cmd, unsigned char addr, int speed,void *other )
{
	unsigned char CmdBuffer[8];
	int *tiltSpeed = (int*)other;
	
	CmdBuffer[0] = 0xa0;
	CmdBuffer[1] = addr-1;
	CmdBuffer[2] = 0;
	CmdBuffer[3] = 0;
	CmdBuffer[4] = 0;
	CmdBuffer[5] = 0;
	switch(cmd)
	{
		case PCT_STOP:
			break;
		case PCT_UP:
			CmdBuffer[3] = 0x08;
			CmdBuffer[5] = speed;
			break;
		case PCT_DOWN:
			CmdBuffer[3] = 0x10;
			CmdBuffer[5] = speed;
			break;
		case PCT_LEFT:
			CmdBuffer[3] = 0x04;
			CmdBuffer[4] = speed;
			break;
		case PCT_RIGHT:
			CmdBuffer[3] = 0x02;
			CmdBuffer[4] = speed;
			break;
		case PCT_LEFT_UP:
			CmdBuffer[3] = 0x0c;
			CmdBuffer[4] = speed;
			CmdBuffer[5] = *tiltSpeed;
			break;
		case PCT_RIGHT_UP:
			CmdBuffer[3] = 0x0a;
			CmdBuffer[4] = speed;
			CmdBuffer[5] =  *tiltSpeed;
			break;
		case PCT_LEFT_DOWN:
			CmdBuffer[3] = 0x14;
			CmdBuffer[4] = speed;
			CmdBuffer[5] =  *tiltSpeed;
			break;
		case PCT_RIGHT_DOWN:
			CmdBuffer[3] = 0x12;
			CmdBuffer[4] = speed;
			CmdBuffer[5] =  *tiltSpeed;
			break;
		
		case PCT_IRIS_INC:	
			CmdBuffer[2] = 0x08;
			break;	
		case PCT_IRIS_DEC:
			CmdBuffer[2] = 0x04;
			break;
		case PCT_ZOOM_DEC:
			CmdBuffer[3] = 0x40;
			break;
		case PCT_ZOOM_INC:
			CmdBuffer[3] = 0x20;
			break;		
		case PCT_FOCUS_INC:
			CmdBuffer[2] = 0x02;
			break;
		case PCT_FOCUS_DEC:
			CmdBuffer[2] = 0x01;
			break;
			
		case PCT_SET_PRESET:
			CmdBuffer[3] = 0x03;
			CmdBuffer[5] = speed;
			break;
		case PCT_CLR_PRESET:
			CmdBuffer[3] = 0x05;
			CmdBuffer[5] = speed;
			break;
		case PCT_CALL_PRESET:
			CmdBuffer[3] = 0x07;
			CmdBuffer[5] = speed;
			break;
		default:
			printf(("SendCmdToPtzPelco_p not support cmd\n"));
			break;
	}
	CmdBuffer[6] = 0xaf;
	CmdBuffer[7] = CmdBuffer[0]^CmdBuffer[1]^CmdBuffer[2]^CmdBuffer[3]^CmdBuffer[4]^CmdBuffer[5]^CmdBuffer[6];
	SendCmdBySerial(CmdBuffer, 8);
	
	return 0;
}


/*********************************************************************
	Function:   
	Description:   
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:       
  	author:
 ********************************************************************/
HI_S32 HIDVR_HAL_SendCmdToPtz(unsigned char protocol, unsigned char addr, unsigned char cmd, int speed,void *other)
{
	if(g_S32Ptzfd < 0)
	{
		printf(("SendCmdToPtz you must open the ptz serial first!\n"));
		return -1;
	}
	switch(protocol)
	{
		case PP_PELCO_D:
			return HIDVR_HAL_SendCmdToPtzPelco_d(cmd, addr, speed,other);
			break;
		case PP_PELCO_P:
			return HIDVR_HAL_SendCmdToPtzPelco_p(cmd, addr,speed,other);
			break;
		default:
			printf(("SendCmdToPtz not support ptz protocol\n"));
			break;
	}

	return -1;
}
#endif

