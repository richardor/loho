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
  2.Date        : 2010/02/03
    Author      : z50825
    Modification: add english comment	
******************************************************************************/

#ifndef _DEV_HAL_H
#define _DEV_HAL_H

#include "hi_type.h"
#include "keyvaluetable.h"

#define DVR_INFO(fmt...)      \
do{ printf(fmt);\
    printf("%s(%s):%d\n", __func__, __FILE__, __LINE__);\
}while(0)

typedef enum
{
    HAL_VOUT =0,
    HAL_VSPOT=1,
    HAL_VGA  =2,
    HAL_MAX =3
}HAL_OUTPUT_DEV_E;

typedef struct
{
    HI_U32  second;
    HI_U32  minute;
    HI_U32  hour;
    HI_U32  date;
    HI_U32  weekday;
    HI_U32  month;
    HI_U32  year;
}HAL_RTC_TIME_S;

typedef struct 
{
	HI_S32  baud;
	HI_U8 	databits;
	HI_U8 	parity;
	HI_U8 	stopbits;
}ptzserialconfig_t;

typedef enum
{
	DVR_TVSYSTEM_PAL,
	DVR_TVSYSTEM_NTSC,	
}HAL_TV_SYSTEM_E;

typedef enum
{
	HAL_DEV_VGA,
	HAL_DEV_HDMI,
}HAL_VGA_DEV_E;

/******************************

******************************/
#define USE_SENSOR_HAL 
#define USE_PTZ_HAL    

#define PTZ_PROTOCOL_D   "Pelco-D"
#define PTZ_PROTOCOL_P   "Pelco-P"

#define MAX_CHN_NUM  8

#ifdef __cplusplus
extern "C"
{
#endif	


/************************ rtc interface **************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_RTC_Open
	Description:   
		initialize rtc
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_RTC_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_RTC_Close
	Description:   
		initialize rtc
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:    

 ********************************************************************/
HI_VOID HIDVR_HAL_RTC_Close(HI_VOID);



/*********************************************************************
	Function:   
		HIDVR_HAL_RTC_SetTime
	Description:   
		set rtc time
	Calls:		 
  	Called By:     
	parameter:
		ptime:  time to set	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_RTC_SetTime(HAL_RTC_TIME_S* pstTime);

/*********************************************************************
	Function:   
		HIDVR_HAL_RTC_GetTime
	Description:   
		get rtc time
	Calls:		 
  	Called By:     
	parameter:
		ptime:	time to get	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_RTC_GetTime(HAL_RTC_TIME_S* pstTime);


/************************ watchdog interface **************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Open
	Description:   
		initialize watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_WDT_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Close
	Description:   
		deinitlize watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_SetTimeout
	Description:   
		set watchdog timeout
	Calls:		 
  	Called By:     
	parameter:
		u32Msec	:timeout (msec)	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_SetTimeout(HI_U32 u32Msec);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Enable
	Description:   
		enable watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_Enable(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Disable
	Description:   
		disable watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_Disable(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Feed
	Description:   
		feed watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_Feed(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_WDT_Reset
	Description:   
		reset watchdog
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_WDT_Reset(HI_VOID);

/************************ video AD interface **************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_Open
	Description:   
		initialize AD
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_VAD_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_Close
	Description:   
		deinitialize AD
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:     

 ********************************************************************/ 
HI_VOID HIDVR_HAL_VAD_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_SetTvSystem
	Description:   
		set AD norm
	Calls:		 
  	Called By:     
	parameter:
		enTvsystem	:norm to set
				DVR_TYSYSTEM_NTSC
    			DVR_TYSYSTEM_PAL	       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_VAD_SetTvSystem(HAL_TV_SYSTEM_E enTvsystem);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_GetVideoLoss
	Description:   
		get video loss
	Calls:		 
  	Called By:     
	parameter:
		S32Chn(in parameter): 	channel
		pbLoss(out parameter): 	loss identifier
			1 video ok
			0 video loss		       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VAD_GetVideoLoss(HI_S32 s32Chn ,HI_BOOL *pbLoss);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_SetBrightness
	Description:   
		set video brightness
	Calls:		 
  	Called By:     
	parameter:
		S32Chn		channel
		u32Brightness	brightness		       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VAD_SetBrightness(HI_S32 s32Chn, HI_U32 u32Brightness);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_SetSaturation
	Description:   
		set video saturation
	Calls:		 
  	Called By:     
	parameter:
		S32Chn		channel
		u32Saturation	saturation		       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VAD_SetSaturation(HI_S32 s32Chn, HI_U32 u32Saturation);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_SetContrast
	Description:   
		set video contrast
	Calls:		 
  	Called By:     
	parameter:
		S32Chn		channel
		u32Contrast	contrast		       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VAD_SetContrast(HI_S32 s32Chn, HI_U32 u32Contrast);

/*********************************************************************
	Function:   
		HIDVR_HAL_VAD_SetHuePlus
	Description:   
		set video hue
	Calls:		 
  	Called By:     
	parameter:
		S32Chn		channel
		u32HuePlus	hue		       
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VAD_SetHuePlus(HI_S32 s32Chn, HI_U32 u32HuePlus);

/********************** video DA interface ************************/

/*********************************************************************
	this class of interfaces are simliar to those of AD
 ********************************************************************/
HI_S32 HIDVR_HAL_VDA_Open(HI_VOID);
HI_VOID HIDVR_HAL_VDA_Close(HI_VOID);
HI_S32 HIDVR_HAL_VDA_SetTvSystem(HAL_TV_SYSTEM_E enTvsystem);


/********************** audio AD interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_AAD_Open
	Description:   
		initialize AD
	Calls:		 
  	Called By:     
	parameter:
		notice:be careful of the following:
		s32SampleRate 	samplerate
		s32BitWidth 	bitwith	
		s32ChnNum 	    channel	
		s32Seq	      	channel sequence defined in AD 
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_AAD_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_AAD_Close
	Description:   
		initialize AD
	Calls:		 
  	Called By:     
	parameter:
  	Return:  

 ********************************************************************/ 
HI_VOID HIDVR_HAL_AAD_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_AUDIO_PLAYBACK
	Description:   
		select the audio out path for each output device
	Calls:		 
  	Called By:     
	parameter:
		s32Chn 	channel
		enDev	output device
		bLoop 	weather loop in DA 
			1 loop in DA,instead of outputing from i2s
			0 output from i2s
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_AUDIO_PLAYBACK(HI_S32 s32Chn, HI_S32 s32DevId, HI_BOOL bLoop);

/*********************************************************************
	Function:   
		HIDVR_HAL_AAD_Enable_ChnMute
	Description:   
		mute
	Calls:		 
  	Called By:     
	parameter:
		s32Chn 	chanenl
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_AAD_Enable_ChnMute(HI_S32 s32Chn);

/*********************************************************************
	Function:   
		HIDVR_HAL_AAD_Disable_ChnMute
	Description:   
		disable mute
	Calls:		 
  	Called By:     
	parameter:
		s32Chn 	channel
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_AAD_Disable_ChnMute(HI_S32 s32Chn);

/*********************************************************************
	Function:   
		HIDVR_HAL_AAD_SetVolum
	Description:   
		set volume of channel
	Calls:		 
  	Called By:     
	parameter:
		s32Chn	channel
		u32Vol	volume
  	Return:
  		success 0
  		fail -1   

 ********************************************************************/
HI_S32 HIDVR_HAL_AAD_SetVolum(HI_S32 s32Chn, HI_U32 u32Vol);


/********************** audio DA interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_ADA_Open
	Description:   
		initialize DA
	Calls:		 
  	Called By:     
	parameter:
		notice the following:
		s32SampleRate 	samplerate
		s32BitWidth 	bitwith
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_ADA_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_ADA_Close
	Description:   
		deinitialize DA
	Calls:		 
  	Called By:     
	parameter:
  	Return:  

 ********************************************************************/ 
HI_VOID HIDVR_HAL_ADA_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_ADA_SetVolum
	Description:   
		set da volume
	Calls:		 
  	Called By:     
	parameter:
		s32Chn 	channel
		enDev	out put device
		u32Vol 	volume
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_ADA_SetVolum(HI_S32 s32Chn, HI_S32 s32DevId, HI_U32 u32Vol);

/*********************************************************************
	Function:   
		HIDVR_HAL_ADA_Enable_Mute
	Description:   
		DA mute
	Calls:		 
  	Called By:     
	parameter:
		enDev 	device
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_ADA_Enable_Mute(HI_S32 s32DevId);

/*********************************************************************
	Function:   
		HIDVR_HAL_ADA_Disable_Mute
	Description:   
		DA disable mute
	Calls:		 
  	Called By:     
	parameter:
		enDev 	device
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_ADA_Disable_Mute(HI_VOID);


/********************** alarm interface ************************/
#ifdef USE_SENSOR_HAL
/*********************************************************************
	Function:   
		HIDVR_HAL_INDEV_ALARM_Open
	Description:   
		initialize alarm input
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_INDEV_ALARM_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_INDEV_ALARM_Close
	Description:   
		deinitialize alarm input
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_INDEV_ALARM_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_INDEV_ALARM_GetState
	Description:   
		get alarm input status
	Calls:		 
  	Called By:     
	parameter:
		s32Chn		channel
		ps32Alarm 	status to get
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_INDEV_ALARM_GetState(HI_S32 s32Chn, HI_S32* ps32Alarm);


/********************** alarm interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_OUTDEV_ALARM_Open
	Description:   
		initialize alarm out
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_OUTDEV_ALARM_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_OUTDEV_ALARM_Close
	Description:   
		deinitial alarm out 
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_OUTDEV_ALARM_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_OUTDEV_ALARM_OutData
	Description:   
		set alarm out status
	Calls:		 
  	Called By:     
	parameter:
		s32Chn		channel
		s32Alarm	status to set
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_OUTDEV_ALARM_OutData(HI_S32 s32Chn, HI_S32 s32Alarm);
#endif

/********************** buzzer interface************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_BUZZER_Open
	Description:   
		initialize buzzer
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_BUZZER_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_BUZZER_Close
	Description:   
		deinitialize buzzer
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_BUZZER_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_BUZZER_Enable
	Description:   
		enable buzzer
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_BUZZER_Enable(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_BUZZER_Disable
	Description:   
		disable buzzer
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_BUZZER_Disable(HI_VOID);

/********************** HDD Led interface************************/
/*********************************************************************
	Function:   
		HIDVR_HAL_HDDLED_Open
	Description:   
		initialize HDDLED
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_HDDLED_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_HDDLED_Close
	Description:   
		deinitialize HDDLED
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_HDDLED_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_HDDLED_Enable
	Description:   
		enable HDDLED
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_HDDLED_Enable(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_HDDLED_Disable
	Description:   
		disable HDDLED
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_HDDLED_Disable(HI_VOID);

/********************** KEYBOARD interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_KEYBOARD_Open
	Description:   
		initialize keyboard
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_KEYBOARD_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_KEYBOARD_Close
	Description:   
		deinitialize keyboard
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_KEYBOARD_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_KEYBOARD_ReadKeyVal
	Description:   
		get keyvalue
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue      

 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_KEYBOARD_ReadKeyVal(HI_VOID);
/*********************************************************************
	Function:   
		HIDVR_HAL_KEYBOARD_TransKeyVal
	Description:   
		translate keyvalue according to different status
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue        
 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_KEYBOARD_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate);

int DVR_KEYBOARD_EnableRepKey(int bEnable);
int DVR_KEYBOARD_SetRepTimeout(unsigned int msec);

/*********************************************************************
	Function:   
		HIDVR_HAL_IR_Open
	Description:   
		initialize ir
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       
 ********************************************************************/ 
HI_S32 HIDVR_HAL_IR_Open(HI_VOID);
/*********************************************************************
	Function:   
		HIDVR_HAL_IR_Close
	Description:   
		deinitialize ir
	Calls:		 
  	Called By:     
	parameter:
  	Return:
 ********************************************************************/ 
HI_VOID HIDVR_HAL_IR_Close(HI_VOID);
/*********************************************************************
	Function:   
		HIDVR_HAL_IR_ReadKeyVal
	Description:   
		get ir keyvalue
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue       

 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_IR_ReadKeyVal(HI_VOID);
/*********************************************************************
	Function:   
		HIDVR_HAL_IR_TransKeyVal
	Description:   
		translate keyvalue according to different status
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue      

 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_IR_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate);

/********************** 485 KEYBOARD interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_485KEYBOARD_Open
	Description:   
		initialize 485keyboard
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
HI_S32 HIDVR_HAL_485KEYBOARD_Open(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_485KEYBORD_Close
	Description:   
		deinitialize 485 keyboard
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_485KEYBOARD_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_485KEYBORD_ReadKeyVal
	Description:   
		get 485 keyvalue 
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue      

 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_485KEYBOARD_ReadKeyVal(HI_VOID);
/*********************************************************************
	Function:   
		HIDVR_HAL_485KEYBOARD_TransKeyVal
	Description:   
		translate keyvalue according to different status
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		keyvalue      

 ********************************************************************/ 
keyvaluetable_t HIDVR_HAL_485KEYBOARD_TransKeyVal(keyvaluetable_t key, guistatetable_t guistate);

/********************** PTZ interface ************************/

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Open
	Description:   
		initialize PTZ serial port
	Calls:		 
  	Called By:     
	parameter:
		pstPara	serial port paramerter
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/ 
#ifdef USE_PTZ_HAL
HI_S32 HIDVR_HAL_PTZ_Open(ptzserialconfig_t *pstPara);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Close
	Description:   
		deinitialize ptz serial port
	Calls:		 
  	Called By:     
	parameter:
  	Return:      

 ********************************************************************/
HI_VOID HIDVR_HAL_PTZ_Close(HI_VOID);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Cfg
	Description:   
		set PTZ serial port
	Calls:		 
  	Called By:     
	parameter:
	        S32Chn 	channel
	        ptzserialconfig_t   serial paramerter
  	Return:      
  		success 0
  		fail -1       

 ********************************************************************/
 HI_S32 HIDVR_HAL_PTZ_Cfg(ptzserialconfig_t pstPara);
/*********************************************************************
	Function:   
	Description:   
	Calls:		 
  	Called By:     
	parameter:	       
  	Return:       
  	author:
 ********************************************************************/
 HI_S32 SendCmdBySerial(HI_U8 *pCmdBuffer, HI_S32 len);
/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Stop
	Description:   
		PTZ stop
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Stop(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Left
	Description:   
		PTZ move left
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		u8Addr		ptz address
		s32Speed	ptz speed
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Left(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Right
	Description:   
		PTZ move right
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		u8Addr		ptz address
		s32Speed	ptz speed
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Right(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Up
	Description:   
		PTZ move up
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		u8Addr		ptz address
		s32Speed	ptz speed
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Up(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Down
	Description:   
		PTZ move down
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		u8Addr		ptz addtess
		s32Speed	ptz speed
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Down(HI_S32 S32Chn, HI_U8 u8Addr,  HI_S32 s32Speed);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Iris_Inc
	Description:   
		PTZ iris increase
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		u8Addr		ptz address
		s32Speed	ptz speed
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Iris_Inc(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Iris_Dec
	Description:   
		PTZ iris decrease
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Iris_Dec(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Zoom_Inc
	Description:   
		PTZ zoom in
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Zoom_Inc(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Zoom_Dec
	Description:   
		PTZ zoom out
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Zoom_Dec(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Focus_Inc
	Description:   
		PTZ focus in
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Focus_Inc(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Focus_Dec
	Description:   
		PTZ focus out
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 	channel
		u8Addr	ptz address
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Focus_Dec(HI_S32 S32Chn, HI_U8 u8Addr);

/*********************************************************************
	Function:   
		DVR_HAL_PTZ_Bind_Protocol
	Description:   
		bind protocol
	Calls:		 
  	Called By:     
	parameter:
		S32Chn 		channel
		pu8Protocol	protocol name
		as:
		" pelco-d"
		 "pelco-p"
  	Return:
  		success 0
  		fail -1       
 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Bind_Protocol(HI_S32 S32Chn, HI_U8* pu8Protocol);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Enum_Protocol
	Description:   
		enum all the surpported protocols return in protocol array
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
//HI_U8* HIDVR_HAL_PTZ_Get_Protocol(HI_VOID);
HI_S32 HIDVR_HAL_PTZ_Enum_Protocol(HI_U8 protocol[16][64],HI_U32 *pProtocolCnt);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_To_Pos
	Description:   
		PTZ move to preset position
	Calls:		 
  	Called By:     
	parameter:
	        S32Chn :	channel
		u8Addr:	ptz address
		u32Curpt:  current position want to move to
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_To_Pos(HI_S32 S32Chn, HI_U8 u8Addr, HI_U32 u32Curpt);

/*********************************************************************
	Function:   
		HIDVR_HAL_PTZ_Set_Pos
	Description:   
		PTZ preset the position
	Calls:		 
  	Called By:     
	parameter:
	        S32Chn; 	channel
		u8Addr:	address
		u32Curpt:  preset position
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_PTZ_Set_Pos(HI_S32 S32Chn, HI_U8 u8Addr, HI_U32 u32Curpt);
#endif
/*********************************************************************
	Function:   
		HIDVR_HAL_VGA_SetDev
	Description:   
		select the type of VGA output device:
		HAL_DEV_VGA,
		HAL_DEV_HDMI,
	Calls:		 
  	Called By:     
	parameter:
  	Return:
  		success 0
  		fail -1       

 ********************************************************************/
HI_S32 HIDVR_HAL_VGA_SetDev(HAL_VGA_DEV_E enVgaDev);
#ifdef USE_PTZ_HAL
HI_S32 HIDVR_HAL_SendCmdToPtzPelco_d(unsigned char cmd, unsigned char addr,  int speed,void *other );
HI_S32 HIDVR_HAL_SendCmdToPtzPelco_p(unsigned char cmd, unsigned char addr, int speed,void *other );
HI_S32 HIDVR_HAL_SendCmdToPtz(unsigned char protocol, unsigned char addr, unsigned char cmd, int speed,void *other);
#endif


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
HI_S32 HIDVR_HAL_SYS_GetSerialCode();

#ifdef __cplusplus
}
#endif

#endif //_DEV_HAL_H

