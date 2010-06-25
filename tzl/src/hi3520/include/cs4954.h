 /*     ~/source/drv/cs4954.h for Linux .
 *
 * 
 *  This file defines cs4954 micro-definitions for user.
 *
 * History:
 *     27-Sep-2008 create this file.
 *      
 */

#ifndef _CS4955_H_
	#define _CS4955_H_

	#define 	OUTPU_VIDEO_NORM_PAL		0
	#define 	OUTPUT_VIDEO_NORM_NTSC		1

	#define	CS4954_RESET	0x01
	#define	CS4954_INIT 	0x02

	#define	CS4954_READ_REG	0x0a
	#define	CS4954_WRITE_REG 	0x0b

	#define	CS4954_SET_GPIO_DIR		0x10
	#define	CS4954_SET_GPIO_DATA		0x11
	#define	CS4954_GET_GPIO_DATA		0x12


	struct cs4954_write_reg
	{
	    unsigned char addr;
	    unsigned char value;
	};

#endif

