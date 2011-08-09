/*   extdrv/peripheral/dc/mt9v131.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 *
 * History:
 *     04-Apr-2006 create this file
 *
 */
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/config.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>

#include <asm/hardware.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>

#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/kcom.h>
#include <kcom/gpio_i2c.h>

#include "mt9d131.h"


/* mt9d131 i2c slaver address micro-definition. */
#define I2C_MT9D131     0x90
static int out_mode = 1;
static int powerfreq = DC_VAL_50HZ;


static void mt9d131_default_init(void)
{
    /*========soft reset===============*/
    /* page 0 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    
    gpio_i2c_write(I2C_MT9D131,0x65, 0xa0);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00); 
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);    
    
    gpio_i2c_write(I2C_MT9D131,0xc3, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);  
    /* page 0 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00); 
    
    gpio_i2c_write(I2C_MT9D131,0x0d, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x21);   
    
    msleep(10);  
    
    gpio_i2c_write(I2C_MT9D131,0x0d, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);   
    
    msleep(10); 
    /* soft reset end */ 
      
    /* pll control */
    gpio_i2c_write(I2C_MT9D131,0x66, 0x10);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
    
    gpio_i2c_write(I2C_MT9D131,0x67, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00); 
    
    msleep(50); 
        
    gpio_i2c_write(I2C_MT9D131,0x65, 0xA0);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    
    msleep(50);      
    
    gpio_i2c_write(I2C_MT9D131,0x65, 0x20);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);   
    
    msleep(100);      
    /* pll control end */

/**************set mode*******************************************/   
    gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);   
    /* contexa/b  bypass jpeg */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0B);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x30);

    /* page 0 */
    gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00); 
    /* Read Mode (A) */ 
    gpio_i2c_write(I2C_MT9D131, 0x21, 0x03);    
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    
    
/************set mode end******************************************/    
  
 /************************flicker detection****************************************/
     gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00); 
     gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01); 
 
 /*  search_f1_50  Lower limit of period range  30 */       
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA4);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x08);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x24);
 /*   search_f1_50  upper limit of period range  32 */ 
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA4);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x09);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x26);
   /*  search_f1_60  Lower limit of period range  37  */   
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA4);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0a);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x1e);
  /*  search_f1_60  upper limit of period range  39  */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA4);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0b);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x20);
   /* R9_Step_60   minimal shutter width step for 60hz ac  157 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x24);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x9c);
  /* R9_Step_50   minimal shutter width step for 50hz ac  188 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x24);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x13);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xbc);
          
    if(powerfreq == DC_VAL_50HZ)
    {   
         /*====fix to 50HZ====*/
        gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);
    
        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x2a);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x02);
    
        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa4);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0xc0);
    
        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x06);
    }
    else if(powerfreq == DC_VAL_60HZ)
    {
        
         /*====fix to 60HZ====*/
        gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);

        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x2a);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x02);

        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa4);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x80);

        gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
        gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
        gpio_i2c_write(I2C_MT9D131,0xf1, 0x06);
    }
   /************************************flicker detection  end ****************************/ 
    /***************************************************set auto exposure*********/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00); 
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01); 
    /* Max R12 (B)(Shutter Delay)  402 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x22);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0b);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x01);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x92);
    /* IndexTH23  Zone number to start gain increase in low-light. 
       Sets  frame rate at normal illumination.   4 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA2);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x17);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
    /* RowTime (msclk per)/4  Row time divided by 4 (in master clock periods)  527 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x22);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x28);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0f);
    /* R9 Step   Integration time of one zone  156 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x22);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2f);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x9c);
    /* Maximum allowed zone number (that is maximumintegration time)  4 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xa2);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0e);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
    /***************************************************set auto exposure  end*********/    
    /*======******************************************lens correcton***************************/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x02);
    
    gpio_i2c_write(I2C_MT9D131, 0x80, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xE8); 	/* LENS_CORRECTION_CONTROL */
    
    gpio_i2c_write(I2C_MT9D131, 0x81, 0x5C);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x2E); 	/* ZONE_BOUNDS_X1_X2 */
    
    gpio_i2c_write(I2C_MT9D131, 0x82, 0x36);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x8A); 	/* ZONE_BOUNDS_X0_X3 */
    
    gpio_i2c_write(I2C_MT9D131, 0x83, 0xA2);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x6C); 	/* ZONE_BOUNDS_X4_X5 */   
    
    gpio_i2c_write(I2C_MT9D131, 0x84, 0x4B);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x26); 	/* ZONE_BOUNDS_Y1_Y2 */
    
    gpio_i2c_write(I2C_MT9D131, 0x85, 0x25);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x71); 	/* ZONE_BOUNDS_Y0_Y3  */ 
    
    gpio_i2c_write(I2C_MT9D131, 0x86, 0x70);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x4B); 	/* ZONE_BOUNDS_Y4_Y5  */
    
    gpio_i2c_write(I2C_MT9D131, 0x87, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xF0); 	/* CENTER_OFFSET */
    
    gpio_i2c_write(I2C_MT9D131, 0x88, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x02); 	/* FX_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x8B, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x20); 	/* FY_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x8E, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xE2); 	/* DF_DX_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x91, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x20); 	/* DF_DY_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x94, 0xC4);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xB8); 	/* SECOND_DERIV_ZONE_0_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x97, 0x25);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x8E); 	/* SECOND_DERIV_ZONE_1_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x9A, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xEB); 	/* SECOND_DERIV_ZONE_2_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x9D, 0x26);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x2C); 	/* SECOND_DERIV_ZONE_3_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0xA0, 0x2C);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05); 	/* SECOND_DERIV_ZONE_4_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0xA3, 0x09);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x25); 	/* SECOND_DERIV_ZONE_5_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0xA6, 0xEE);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xEB); 	/* SECOND_DERIV_ZONE_6_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0xA9, 0xD9);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xB4); 	/* SECOND_DERIV_ZONE_7_RED */
    
    gpio_i2c_write(I2C_MT9D131, 0x89, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x02); 	/* FX_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x8C, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x1B); 	/* FY_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x8F, 0x04);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x29); 	/* DF_DX_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x92, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xF5); 	/* DF_DY_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x95, 0xAE);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xAE); 	/* SECOND_DERIV_ZONE_0_GREEN */  
    
    gpio_i2c_write(I2C_MT9D131, 0x98, 0xFA);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x9C); 	/* SECOND_DERIV_ZONE_1_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x9B, 0x05);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xE3); 	/* SECOND_DERIV_ZONE_2_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x9E, 0x17);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x2F); 	/* SECOND_DERIV_ZONE_3_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0xA1, 0x2A);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00); 	/* SECOND_DERIV_ZONE_4_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0xA4, 0x14);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x23); 	/* SECOND_DERIV_ZONE_5_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0xA7, 0xE8);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xDE); 	/* SECOND_DERIV_ZONE_6_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0xAA, 0xAF);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xAA); 	/* SECOND_DERIV_ZONE_7_GREEN */
    
    gpio_i2c_write(I2C_MT9D131, 0x8A, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x02); 	/* FX_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x8D, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x0E); 	/* FY_BLUE  */ 
    
    gpio_i2c_write(I2C_MT9D131, 0x90, 0x04);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x6B); 	/* DF_DX_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x93, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x92); 	/* DF_DY_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x96, 0xBC);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xBF); 	/* SECOND_DERIV_ZONE_0_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x99, 0x0B);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x8E); 	/* SECOND_DERIV_ZONE_1_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x9C, 0xFF);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xDB); 	/* SECOND_DERIV_ZONE_2_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0x9F, 0x11);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x29); 	/* SECOND_DERIV_ZONE_3_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0xA2, 0x24);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xF2); 	/* SECOND_DERIV_ZONE_4_BLUE  */ 
    
    gpio_i2c_write(I2C_MT9D131, 0xA5, 0x10);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x23); 	/* SECOND_DERIV_ZONE_5_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0xA8, 0xC9);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xE2); 	/* SECOND_DERIV_ZONE_6_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0xAB, 0xCA);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xB0); 	/* SECOND_DERIV_ZONE_7_BLUE */
    
    gpio_i2c_write(I2C_MT9D131, 0xAC, 0x80);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00); 	/* X2_FACTORS */
    
    gpio_i2c_write(I2C_MT9D131, 0xAD, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00); 	/* GLOBAL_OFFSET_FXY_FUNCTION */
    
    gpio_i2c_write(I2C_MT9D131, 0xAE, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x8E); 	/* K_FACTOR_IN_K_FX_FY */
    
    gpio_i2c_write(I2C_MT9D131, 0x08, 0x01);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xFC); 	/* COLOR_PIPELINE_CONTROL */
      
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);
    /* enable  lc/gamma/color corretion */
    gpio_i2c_write(I2C_MT9D131, 0x08, 0x01);    
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xFC);
    /* Refresh Sequencer Mode  6 */
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xa1);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x06); 
    /* refresh sequencer 5 */
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xa1);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05);       
    /*********************************************lens correction end***********/
    
    /********************************gamma and contrast****************************/
    
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA7);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x43);  /* MCU_ADDRESS [MODE_GAM_CONT_A] */
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x12);  /* MCU_DATA_0  */  
    
 
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);  /* MCU_ADDRESS [SEQ_CMD] */
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05);  /* MCU_DATA_0 */
     /**************************************gamma and contrast end***************************/

    /*--------use preview mode, not capture mode--------*/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);
    /* capture  clear */
    gpio_i2c_write(I2C_MT9D131, 0xc6, 0xa1);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x20); 
    gpio_i2c_write(I2C_MT9D131, 0xc8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    /* cmd do preview */
    gpio_i2c_write(I2C_MT9D131, 0xc6, 0xa1);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03); 
    gpio_i2c_write(I2C_MT9D131, 0xc8, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);

}

static void mt9d131_vga_init(void)
{
    /************************************************set mode*********************************/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    /* HBLANK (A) = 174 */
    gpio_i2c_write(I2C_MT9D131, 0x07, 0x00);    
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0xae);
     /* VBLANK (A) = 16 */
    gpio_i2c_write(I2C_MT9D131, 0x08, 0x00);   
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x10);
    
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);   

     /* output_width 800 a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x20);
    /* output_height 600 a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x58);
    /* first sensor-readout row 28  context a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0F);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x1C);
    /* first sensor-readout column 60 context a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x3C);
    /* contexta number of sensor-readout rows 600 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x13);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x58);
    /* contexta number of sensor-readout columns 800 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x15);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x20);
    
    /* extra sensor delay per frame context a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x17);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x18);
    /* row-speed context a 17 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x19);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    /* Crop_X0 (A)  0 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /* Crop_X1 (A)  800 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x29);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x03);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x20);
    /* Crop_Y0 (A)  0 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2b);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /* Crop_Y1 (A)  600 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x58);
    /* FIFO_Conf1 (A)   57570 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0xE0);       
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe2);
    /* FIFO_Conf2 (A)   225 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA7);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6f);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe1);
    /***************************************************set mode end*************/

    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01); 

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer Mode */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 6 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x06);

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 5 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05);

}

static void mt9d131_xxga_init(void)
{
    mt9d131_default_init();
    /************************************************set mode*********************************/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    /* HBLANK (A) = 286 */
    gpio_i2c_write(I2C_MT9D131, 0x07, 0x01);    
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x1e);/* 1280x720 */
    
     /* VBLANK (A) = 119 */
    gpio_i2c_write(I2C_MT9D131, 0x08, 0x00);          
    //gpio_i2c_write(I2C_MT9D131, 0xf1, 0x0b);/* 33fps */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x77);/* 30fps */
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);   

     /* output_width 1600 a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
      
    /* output_height 1200 a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xd0);
        
    /* first sensor-readout row 28  context a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0F);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x1C);
    /* first sensor-readout column 60 context a */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x3C);
    /* contexta number of sensor-readout rows 720 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x13);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xd0);
    /* contexta number of sensor-readout columns 1280 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x15);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
      
    
    /* extra sensor delay per frame context a 155 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x17);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x9b);
    /* row-speed context a 17 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x19);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    /* Crop_X0 (A)  0 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /* Crop_X1 (A)  1600 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x29);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /* Crop_Y0 (A)  0 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2b);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /* Crop_Y1 (A)  1200 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x02);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xd0);
    /* FIFO_Conf1 (A)   57569 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0xE0);       
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe1);

    
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA7);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6f);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe1);
    /***************************************************set mode end*************/

    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01); 

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer Mode */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 6 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x06);

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 5 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05);
}


static void mt9d131_uxga_init(void)
{
    mt9d131_default_init();
    /************************************************set mode*********************************/
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    /* HBLANK (A) = 516 */
    gpio_i2c_write(I2C_MT9D131, 0x07, 0x02);    
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x04);
    /* VBLANK (A) = 47 */
    gpio_i2c_write(I2C_MT9D131, 0x08, 0x00);   
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x2f);
    
    /* page 1 */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01);   

    /*output_width 1600 a*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x06);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x40);
    /*output_height 1200 a*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x05);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x04);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xb0);
    /*first sensor-readout row 28  context a*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x0F);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x1C);
    /*first sensor-readout column 60 context a*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x3C);
    /*contexta number of sensor-readout rows 1200 */
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x13);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x04);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xb0);
    /*contexta number of sensor-readout columns 1600*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x15);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x06);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x40);
    
    /*extra sensor delay per frame context a 1046*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x17);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x04);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x16);
    /*row-speed context a 17*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x19);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x11);
    /*Crop_X0 (A)  0*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x27);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /*Crop_X1 (A)  1600*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x29);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x06);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x40);
    /*Crop_Y0 (A)  0*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2b);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x00);
    /*Crop_Y1 (A)  1200*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x2d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x04);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xb0);
    /*FIFO_Conf1 (A)   57569*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0x27);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6d);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0xE0);       
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe1);
    /*FIFO_Conf2 (A)   225*/
    gpio_i2c_write(I2C_MT9D131,0xC6, 0xA7);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0x6f);
    gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);        
    gpio_i2c_write(I2C_MT9D131,0xf1, 0xe1);
    /***************************************************set mode end*************/

    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x01); 

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer Mode */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 6 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x06);

    msleep(500);     /* DELAY = 500 */
    
    gpio_i2c_write(I2C_MT9D131, 0xC6, 0xA1);    /* Refresh Sequencer */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x03);
    gpio_i2c_write(I2C_MT9D131, 0xC8, 0x00);    /*  = 5 */
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x05);
}

/*
 * mt9d131 open routine. 
 * do nothing.
 *
 */
int mt9d131_open(struct inode * inode, struct file * file)
{
    return 0;
}

/*
 * mt9d131 close routine. 
 * do nothing.
 *
 */
 
int mt9d131_close(struct inode * inode, struct file * file)
{
    return 0;
}


/*
 * mt9d131 ioctl routine. 
 * @param inode: pointer of the node;
 * @param file: pointer of the file;
 *
 * @param cmd: command from the app:
 * 
 * @param arg:arg from app layer.
 *
 * @return value:0-- set success; 1-- set error. 
 *
 */
 
int mt9d131_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;
    unsigned int val = 0;
    
    if (copy_from_user(&val, argp, sizeof(val))) 
    {
        return -EFAULT;
    }
    
    switch (cmd)
    {
        case DC_SET_IMAGESIZE:
        {
            unsigned int imagesize = val;
            if(imagesize == DC_VAL_VGA)
            {
                 mt9d131_vga_init();

            }
            if(imagesize == DC_VAL_XXGA)
            {
                 mt9d131_xxga_init();

            }
            else if(imagesize == DC_VAL_UXGA)
            {
                 mt9d131_uxga_init();
            }
            else
            {
                printk("imagesize_set_error.\n");
                return -1;

            }   
            break;
        }
        
        case DC_SET_POWERFREQ:
        {
            unsigned int powerfreq_dy = val;
            if(powerfreq_dy == DC_VAL_50HZ)
            {
                /*====fix to 50HZ====*/
                gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x2a);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x02);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa4);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0xc0);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x06);
                powerfreq = DC_VAL_50HZ;
            }
            else if(powerfreq_dy == DC_VAL_60HZ)
            {
                /*====fix to 60HZ====*/
                gpio_i2c_write(I2C_MT9D131,0xf0, 0x00);
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x01);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x2a);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x02);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa4);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x04);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x80);
        
                gpio_i2c_write(I2C_MT9D131,0xC6, 0xa1);        
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x03);
                gpio_i2c_write(I2C_MT9D131,0xC8, 0x00);       
                gpio_i2c_write(I2C_MT9D131,0xf1, 0x06);
                powerfreq = DC_VAL_60HZ;
            }
            else 
            {
                printk("powerfreq_set_error.\n");
                return -1;
                
            }
            break;
        }
         
        default:
            return -1;  
    }     
    return 0;
}






/*
 *  The various file operations we support.
 */
 
static struct file_operations mt9d131_fops = {
  .owner    = THIS_MODULE,
  .ioctl    = mt9d131_ioctl,
  .open   = mt9d131_open,
  .release  = mt9d131_close
};

static struct miscdevice mt9d131_dev = {
  MISC_DYNAMIC_MINOR,
  "mt9d131",
  &mt9d131_fops,
};

static int mt9d131_device_init(void)
{
    unsigned char regvalue;
    int loop1;
     
    /* read Chip version */
    gpio_i2c_write(I2C_MT9D131, 0xf0, 0x00);
    gpio_i2c_write(I2C_MT9D131, 0xf1, 0x00);
    regvalue = gpio_i2c_read(I2C_MT9D131, 0x00);
    loop1 = gpio_i2c_read(I2C_MT9D131, 0xf1); 
    if((regvalue != 0x15) || (loop1 != 0x19))
    {
        printk("read Prodect ID Number MSB is %x\n",regvalue);
        printk("read Prodect ID Number LSB is %x\n",loop1);
        printk("check mt9d131 ID error.\n");
        return -EFAULT;
    }
    if(out_mode == 1)
        mt9d131_uxga_init();
    else
        mt9d131_vga_init();  

    return 0;
}

DECLARE_KCOM_GPIO_I2C();
static int __init mt9d131_init(void)
{  
    int ret = 0;
    ret = KCOM_GPIO_I2C_INIT();
    if(ret)
    {
        printk("GPIO I2C module is not load.\n");
        return -1;
    }                 
    ret = misc_register(&mt9d131_dev);
    if(ret)
    {
        KCOM_GPIO_I2C_EXIT();
        printk("could not register mt9v131 devices. \n");
        return ret;
    }
    
    if(mt9d131_device_init()<0)
    {
        misc_deregister(&mt9d131_dev);
        KCOM_GPIO_I2C_EXIT();
        printk("mt9v131 driver init fail for device init error!\n");
        return -1;
    } 
	   
    printk("mt9v131 driver init successful!\n");
    
    return ret;
}

static void __exit mt9d131_exit(void)
{
    misc_deregister(&mt9d131_dev);
    KCOM_GPIO_I2C_EXIT();

}



module_init(mt9d131_init);
module_exit(mt9d131_exit);

#ifdef MODULE
#include <linux/compile.h>
#endif

module_param(out_mode, int, S_IRUGO);
module_param(powerfreq, int, S_IRUGO);

MODULE_INFO(build, UTS_VERSION);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");





