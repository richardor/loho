/******************************************************************************

  Copyright (C), 2001-2011, SZ-Streaming Tech. Co., Ltd.

 ******************************************************************************
  File Name     : HI3120.H
  Version       : 00.01
  Author        : sz-streaming.com
  Created       : 2007/8/29
  Description   : 
  History       :
  1.Date        : 
    Author      : lf
    Modification: Created file

******************************************************************************/

#ifndef _HW3120_H_
#define _HW3120_H_

#define DEV_3120     0xa0

#define HW3120_I2C_WRITE_ERROR	0x55
#define HW3120_I2C_READ_ERROR		0xAA
#define HW3120_I2C_BEGIN	0xFF

#define DETECT_RGB			0x00
#define DETECT_YCBCR		0x01

#define RGB_IN_NUM 		16
#define YCBCR_IN_NUM 	12

#define FREQ_FACTOR_H 20
#define FREQ_FACTOR_V 6

#define FREQ_FACTOR 10

#define INPUT_NONE	0xAA

#define INPUT_VGA60		0x00
#define INPUT_VGA72		0x01
#define INPUT_VGA75		0x02
#define INPUT_VGA85		0x03
    
#define INPUT_SVGA56	0x04
#define INPUT_SVGA60	0x05
#define INPUT_SVGA70  	0x06
#define INPUT_SVGA75	0x07
#define INPUT_SVGA85	0x08


#define INPUT_XGA60  	0x09
#define INPUT_XGA70		0x0a
#define INPUT_XGA75		0x0b
#define INPUT_XGA80		0x0c
#define INPUT_XGA85		0x0d

#define INPUT_DOS1		0x0e
#define INPUT_DOS2		0x0f

#define INPUT_576P50		0x10
#define INPUT_480P60		0x11

#define INPUT_576I50		0x12
#define INPUT_480I60		0x13
    	
#define INPUT_720P60		0x14
#define INPUT_720P50		0x15
	
#define INPUT_1080I50  	0x16
#define INPUT_1080I60  	0x17
	
#define INPUT_1080P25   	0x18
#define INPUT_1080p30 	0x19
     
#define INPUT_1080P50	0x1a
#define INPUT_1080P60	0x1b

//add by lufen 2007/9/3
#define HI3120_SET_BRIGHT 		0x20
#define HI3120_SET_CONTRAST	0x21
#define HI3120_SET_HUE			0x22	
#define HI3120_SET_SAT			0x23	
#define HI3120_INIT				0x24
#define HI3120_SETNORM			0x25
#define HI3120_HV				0x26

#define	MODE_PAL_TO_800x600P60			0
#define	MODE_NTSC_TO_800x600P60			1
#define 	MODE_PAL_TO_1024x768P60		2	
#define 	MODE_NTSC_TO_1024x768P60		3
 #define	MODE_PAL_TO_1366x768P60		4
#define 	MODE_NTSC_TO_1366x768P60		5
#define 	COLORBAR		6
//#define 	MODE_YCBCR_720x576I50_TO_800x600P60				6
//#define		MODE_YCBCR_720x480I60_TO_800x600P60				7
//#define		MODE_RGB_640x480P60_TO_800x600P60				8
//#define		MODE_RGB_800x600P60_TO_800x600P60				9
//#define		MODE_RGB_1024x768P60_TO_800x600P60				10

//#define	_WHITE_BALANCE_
#define	_IMAGE_ENGAGE_
//#define	_GAMMA_
//#define	_LVDS_OUT_
//#define _HI3120_TEST_
//#define _HI3120_RESET_
#define DVR_8204_PRO


typedef struct S_HI3120_SET
{
	unsigned int x;
	unsigned int y;
	unsigned char bir;
	unsigned char con;
	unsigned char hue;
	unsigned char sat;
	unsigned char mode;
	
}STRU_HI3120_SET,*PSTRU_HI3120_SET;

extern unsigned char hw3120_mode(unsigned char mode);
extern unsigned char hw3120_init(void);
extern unsigned char hw3120_BlueScreen(unsigned char value);
extern void hw3120_BlackScreen(unsigned char value);
extern void hw3120_voresync(void);
extern unsigned char hw3120_lvds_enable(unsigned char bonoff);
extern void hw3120_ColorBar(unsigned char value);
extern unsigned char hw3120_itop(void);
extern unsigned char hw3120_device_set(unsigned char ucdevice);
extern unsigned char hw3120_gamma(void);
extern void hw3120_sw_reset(void);
extern void hw3120_lvds_mode(unsigned char bMode);
extern void hw3120_lvds_powerdown(unsigned char bonoff);
extern unsigned char hw3120_i2c_write(unsigned char sub_add,unsigned char i2c_data);
extern unsigned char hw3120_i2c_read(unsigned char sub_add);
unsigned char hw3120_detect(unsigned char ucDetect_Type);
void hw3120_ColorSpace_onoff(unsigned char bvalue);
void	hw3120_image_enhance_onoff(unsigned char bvalue);

void hw3120_Brightness(unsigned char u8value);
void hw3120_Contrast(unsigned char u8value);
void hw3120_Hue(unsigned char value);
void hw3120_Saturation(unsigned char u8value);
#endif
