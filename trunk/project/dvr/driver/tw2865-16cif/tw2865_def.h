


#if 0
unsigned char tbl_pal_tw2865_common[] = {
	// CH1 CH2 CH3 CH4
	0x00,0x00,0x64,0x11,//.0x00~0x03	 0x10~0x13	0x20~0x23  0x30~0x33
	0x80,0x80,0x00,0x12,//.0x04~0x07	 0x14~0x17	0x24~0x27  0x34~0x37
	0x12,0x20,0x0C,0xD0,//.0x08~0x0b	 0x18~0x1b	0x28~0x2b  0x38~0x3b
	0x00,0x00,0x07,0x7F	//.0x0c~0x0f	 0x1c~0x1f	0x2c~0x2f   0x3c~0x3f
};
unsigned char tbl_ntsc_tw2865_common[] = {  
	// CH1 CH2 CH3 CH4
	0x00,0x00,0x64,0x11,//.0x00~0x03	0x10~0x13	0x20~0x23	0x30~0x33
	0x80,0x80,0x00,0x02,//.0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x12,0xF0,0x0C,0xD0,//.0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x00,0x00,0x07,0x7F	//.0x0c~0x0f	0x1c~0x1f	0x2c~0x2f	0x3c~0x3f
};
#else   /* �޸�ͼ���������üĴ��� */
unsigned char tbl_pal_tw2865_common[] = {
	// CH1 CH2 CH3 CH4
	0x00,0x08,0x50,0xb9,//.0x00~0x03	 0x10~0x13	0x20~0x23  0x30~0x33
	0x70,0x70,0x00,0x12,//.0x04~0x07	 0x14~0x17	0x24~0x27  0x34~0x37
	0x17,0x20,0x0C,0xD0,//.0x08~0x0b	 0x18~0x1b	0x28~0x2b  0x38~0x3b
	0x00,0x00,0x07,0x7F	//.0x0c~0x0f	 0x1c~0x1f	0x2c~0x2f   0x3c~0x3f
};
unsigned char tbl_ntsc_tw2865_common[] = {  
	// CH1 CH2 CH3 CH4
	0x00,0x08,0x50,0xb9,//.0x00~0x03	 0x10~0x13	0x20~0x23  0x30~0x33
	0x70,0x70,0x00,0x02,//.0x04~0x07	0x14~0x17	0x24~0x27	0x34~0x37
	0x17,0xF0,0x0C,0xD0,//.0x08~0x0b	0x18~0x1b	0x28~0x2b	0x38~0x3b
	0x00,0x00,0x07,0x7F	//.0x0c~0x0f	0x1c~0x1f	0x2c~0x2f	0x3c~0x3f
};

#endif
/*
unsigned char InitAudioSetup[] = {
	0x33,0x33,0x03,0x10,//...	0xD0~0xD3
	0x32,0x54,0x76,0x98,//...	0xD4~0xD7
	0xba,0xdc,0xfe,0xC1,//...	0xD8~0xDB
	0x00,0x00,0x00,0x80,//...	0xDC~0xDF
	0x14,0xC0,0xAA,0xAA //...	0xE0~0xE3
};
*/
unsigned char InitAudioSetup[] = {
	0x33,0x33,0x03,0x10,//...	0xD0~0xD3
	0x32,0x54,0x76,0x98,//...	0xD4~0xD7
	0xba,0xdc,0xfe,0xC1,//...	0xD8~0xDB
	0x00,0x00,0x00,0x80,//...	0xDC~0xDF
	0x14,0xC0,0xAA,0xAA //...	0xE0~0xE3
};

#if 1
/*audio*/
unsigned char AudioSetup8K[] = {/*0xf0--0xf5*/
	0x83, 0xb5, 0x09, 0x00, 0xa0, 0x00
};

unsigned char AudioSetup16K[] = {/*0xf0--0xf5*/
	0x07, 0x6b, 0x13, 0x00, 0x40, 0x01
};

unsigned char AudioSetup32K[] = {/*0xf0--0xf5*/
	0x0e, 0xd6, 0x26, 0x00, 0x80, 0x02
};


unsigned char AudioSetup44_1K[] = {/*0xf0--0xf5*/
	0x65, 0x85, 0x35, 0x00, 0x72, 0x03	
};

unsigned char AudioSetup48K[] = {/*0xf0--0xf5*/
	0x15, 0x41, 0x3a, 0x00, 0xc0, 0x03	
};


unsigned char tbl_pal_tw2865_8KHz[] = {  
	0x83,0xB5,0x09,0x00,0xA0,0x00			//...0xF0~0xF5
};
unsigned char tbl_ntsc_tw2865_8KHz[] = {  
	0x83,0xB5,0x09,0x78,0x85,0x00			//...0xF0~0xF5
  };
#else
/*audio*/
unsigned char AudioSetup8K[] = {/*0xf0--0xf5*/
	0x83, 0xb5, 0x09, 0x78, 0x85, 0x00
};

unsigned char AudioSetup16K[] = {/*0xf0--0xf5*/
	0x07, 0x6b, 0x13, 0xef, 0x0a, 0x01
};

unsigned char AudioSetup32K[] = {/*0xf0--0xf5*/
	0x0e, 0xd6, 0x26, 0xde, 0x15, 0x02
};


unsigned char AudioSetup44_1K[] = {/*0xf0--0xf5*/
	0x65, 0x85, 0x35, 0xbc, 0xdf, 0x02	
};

unsigned char AudioSetup48K[] = {/*0xf0--0xf5*/
	0x15, 0x41, 0x3a, 0xcd, 0x20, 0x03	
};


unsigned char tbl_pal_tw2865_8KHz[] = {  
	0x83,0xB5,0x09,0x00,0xA0,0x00			//...0xF0~0xF5
};
unsigned char tbl_ntsc_tw2865_8KHz[] = {  
	0x83,0xB5,0x09,0x78,0x85,0x00			//...0xF0~0xF5
  };
#endif


