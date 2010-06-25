#ifndef __TW2865_LINUX__
#define __TW2865_LINUX__


#define TW2865_NTSC	1
#define TW2865_PAL	2

typedef struct _registerrw_{/*register read and write*/
	unsigned char addr;//register address
	unsigned char value;//register value
	unsigned char ChipNo;//chip number, base on zero
	unsigned char reserve[5];
}registerrw_t;

typedef struct _analogsetting_{/*analog set*/
	unsigned char hue;
	unsigned char contrast;
	unsigned char brightness;
	unsigned char saturation;
	unsigned char ChnNo;/*base 0, [0, 15]*/
	unsigned char reserve[7];
	unsigned long SettingBitmap;
}analogsetting_t;


typedef struct _audiooutput_{
	unsigned char PlaybackOrLoop;/*0:playback; 1:loop*/
	unsigned char channel;/*[0, 15]*/
	unsigned char reserve[6];
}audiooutput_t;


typedef enum _audiosamplerate_{
	ASR_8K,
	ASR_16K,
	ASR_32K,
	ASR_44DOT1K,
	ASR_48K
}audiosamplarate_t;

#define TW2865CMD_READ_REG		0x00
#define TW2865CMD_WRITE_REG	0x01
#define TW2865CMD_GET_VL		0x02
#define TW2865CMD_SET_AOSR		0x03//Audio output sample rate
#define TW2865CMD_SET_ANALOG	0x04
#define TW2865CMD_GET_ANALOG	0x05
#define TW2865CMD_READ_REGT	0x06
#define TW2865CMD_AUDIO_OUTPUT	0x07
#define TW2865CMD_AUDIO_MUTE	0x08
#define TW2865CMD_AUDIO_DEMUTE	0x09
#define TW2865CMD_AUDIO_VOLUME	0X0a//…Ë÷√“Ù¡ø

#endif


