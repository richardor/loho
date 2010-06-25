#ifndef __TW2866_H__
#define __TW2866_H__

#include <linux/ioctl.h>

#define TW2866_NTSC	        1
#define TW2866_PAL	        2
#define TW2866_AUTO	        3
#define TW2866_INVALID_MODE	4

typedef struct _tw2866_video_norm
{
    unsigned int chip;
    unsigned int mode;
} tw2866_video_norm;


#define TW2866_1D1_MODE     0
#define TW2866_2D1_MODE     1
#define TW2866_4CIF_MODE    2
#define TW2866_4D1_MODE     3

typedef struct _tw2866_work_mode
{
    unsigned int chip;
    unsigned int mode;
} tw2866_work_mode;


typedef struct _tw2866_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
} tw2866_video_loss;


#define TW2866_SET_HUE          0x01
#define TW2866_SET_CONTRAST     0x02
#define TW2866_SET_BRIGHT       0x04
#define TW2866_SET_SATURATION   0x08

typedef struct _tw2866_image_adjust
{
    unsigned char chip;
    unsigned char chn;
	unsigned char hue;
	unsigned char contrast;
	unsigned char brightness;
	unsigned char saturation;
	unsigned long item_sel;/* use such as "TW2866_SET_HUE|TW2866_SET_CONTRAST" */
} tw2866_image_adjust;

typedef struct _analogsetting_{/*analog set*/
	unsigned char hue;
	unsigned char contrast;
	unsigned char brightness;
	unsigned char saturation;
	unsigned char ChnNo;/*base 0, [0, 15]*/
	unsigned char reserve[7];
	unsigned long SettingBitmap;
}analogsetting_t;


typedef struct _tw2866_audio_output
{
	unsigned char PlaybackOrLoop;   /*0:playback; 1:loop*/
	unsigned char channel;          /*[0, 15]*/
	unsigned char reserve[6];
} tw2866_audio_output;

typedef struct _registerrw_{/*register read and write*/
	unsigned char addr;//register address
	unsigned char value;//register value
	unsigned char ChipNo;//chip number, base on zero
	unsigned char reserve[5];
}registerrw_t;

typedef enum _tw2866_audio_samplerate
{
	TW2866_SAMPLE_RATE_8000,
	TW2866_SAMPLE_RATE_16000,
	TW2866_SAMPLE_RATE_32000,
	TW2866_SAMPLE_RATE_44100,
	TW2866_SAMPLE_RATE_48000,
	TW2866_SAMPLE_RATE_BUTT
} tw2866_audio_samplerate;


#define TW2866_IOC_MAGIC            't'

#define TW2866CMD_READ_REG		    _IOWR(TW2866_IOC_MAGIC, 1, int) 
#define TW2866CMD_WRITE_REG	        _IOW(TW2866_IOC_MAGIC, 2, int) 

//#define TW2866_GET_VIDEO_LOSS		_IOWR(TW2866_IOC_MAGIC, 4, tw2866_video_loss) 
#define TW2866_GET_VIDEO_LOSS		_IOWR(TW2866_IOC_MAGIC, 4, unsigned long) 
#define TW2866_SET_IMAGE_ADJUST	    _IOW(TW2866_IOC_MAGIC, 5, tw2866_image_adjust) 
#define TW2866_GET_IMAGE_ADJUST	    _IOWR(TW2866_IOC_MAGIC, 6, tw2866_image_adjust) 
#define TW2866_SET_VIDEO_NORM	    _IOW(TW2866_IOC_MAGIC, 7, tw2866_video_norm) 
#define TW2866_GET_VIDEO_NORM	    _IOWR(TW2866_IOC_MAGIC, 8, tw2866_video_norm) 
#define TW2866_SET_WORK_MODE	    _IOW(TW2866_IOC_MAGIC, 9, tw2866_work_mode) 

#define TW2866_SET_SAMPLE_RATE	    _IOW(TW2866_IOC_MAGIC, 10, tw2866_audio_samplerate) 

#define TW2866_SET_AUDIO_OUTPUT		_IOW(TW2866_IOC_MAGIC, 11, tw2866_audio_output) 
#define TW2866_SETAUDIO_MUTE		_IOW(TW2866_IOC_MAGIC, 12, tw2866_audio_output) 
#define TW2866_SET_AUDIO_DEMUTE		_IOW(TW2866_IOC_MAGIC, 13, tw2866_audio_output) 
#define TW2866_SET_AUDIO_VOLUME		_IOW(TW2866_IOC_MAGIC, 14, tw2866_audio_output) //…Ë÷√“Ù¡ø

#endif


