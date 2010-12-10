/*
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
 * along with this program; 
 *
 * History: 
 *      10-April-2006 create this file
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>

#include <asm/hardware.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
// #include <linux/kcom.h>

#include "rtc_ds1339.h"
#include "../gpio-i2c/gpio_i2c.h"
// extern unsigned char gpio_i2c_read(unsigned char devaddress, unsigned char address);
// extern void gpio_i2c_write(unsigned char devaddress, unsigned char address, unsigned char value);


#define I2C_DS1339            0xD0


spinlock_t  rtc_lock;
static unsigned long 	rtc_status = 0;

#undef  CONFIG_PROC_FS
#define CONFIG_PROC_FS


int judge_date(int month, int date)
{
	if(date == 31)
	{
		switch(month)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:	return 31;
	
		  	case 4:
		  	case 6:
		  	case 9:
		  	case 11:  return 30;
		  
		  	default:  
	  		break; 
	  }
	}
	return date;
}


/*
 *	set time
 */
int  rtc_set(rtc_time_t compositetime)
{   
	unsigned char date;
	   
	gpio_i2c_write(I2C_DS1339, 0x0, ((compositetime.second/10*16) + (compositetime.second%10))&0x7f);
	gpio_i2c_write(I2C_DS1339, 0x1, ((compositetime.minute/10*16) + (compositetime.minute%10))&0x7f);
	gpio_i2c_write(I2C_DS1339, 0x2, ((compositetime.hour/10*16) + (compositetime.hour%10))&0x3f);
	gpio_i2c_write(I2C_DS1339, 0x3, ((compositetime.weekday/10*16) + (compositetime.weekday%10))&0x7);
	
	date = judge_date(compositetime.month, compositetime.date);
	gpio_i2c_write(I2C_DS1339, 0x4, ((date/10*16) + (date%10))&0x3f);
	
	gpio_i2c_write(I2C_DS1339, 0x05, ((compositetime.month/10*16) + (compositetime.month%10))&0x1f);
	gpio_i2c_write(I2C_DS1339, 0x06, (unsigned char)((compositetime.year - 2000)/10*16 + (compositetime.year - 2000)%10));

    	
	return 0; 	
}                          


/*
 *	get current time
 *	the type of return value is rtc_time_t
 */
int  rtc_get(rtc_time_t *pcompositetime)
{   	

	char second, minute, hour, date, weekday, month, year;
		  
  
	/* get second */
	second = (int)gpio_i2c_read(I2C_DS1339,0x00) & 0x7f ;
	pcompositetime->second = (second & 0x0f) + second /16*10;
    	
	/* get minite */
	minute = gpio_i2c_read(I2C_DS1339,0x01) & 0x7f;
	pcompositetime->minute = (minute&0x0f) + minute/16*10;
    	
	/* get hour */
	hour = gpio_i2c_read(I2C_DS1339,0x02) & 0x3f;
	pcompositetime->hour = (hour&0x0f)+ hour/16*10;

	/* get weekday */
	weekday = gpio_i2c_read(I2C_DS1339,0x03) & 0x07;
	pcompositetime->weekday = (weekday&0x0f) + weekday/16*10;	    	

	
	/* get date */
	date = gpio_i2c_read(I2C_DS1339,0x04) & 0x3f;
	pcompositetime->date = (date&0x0f) + date/16*10;
	
	/* get month */
	month = gpio_i2c_read(I2C_DS1339,0x05) & 0x1f;
	pcompositetime->month = (month&0x0f) + month/16*10;
	

#if 0	//23:59:59-00:00:00跳时天没变
    pcompositetime->date = judge_date(pcompositetime->month, pcompositetime->date);
    gpio_i2c_write(I2C_DS1339, 0x04, ((pcompositetime->date/10*16 + pcompositetime->date%10) & 0x3f));
#endif

	/* get year */
	year = gpio_i2c_read(I2C_DS1339,0x06);
	pcompositetime->year = 2000 + (year&0x0f) + year/16*10;
    	    
	return 0;
}


/*
 *	ioctl function. for usr to control RTC	
 * 	parameter:
 *		RTC_AIE_ON:   enable interrupt
 *		RTC_AIE_OFF:  disable interrupt
 *		RTC_ALM_READ: get current alm time
 *		RTC_ALM_SET:  set alm. type of input is rtc_time_t. 
 *		RTC_RD_TIME:  get current time
 *		RTC_SET_TIME: set time. type of input is rtc_time_t. 
 */
static int rtc_ioctl(struct inode *inode, struct file *file,
		     unsigned int cmd, unsigned long arg)
{
    	rtc_time_t tm;       	
    	void __user *argp = (void __user *)arg; 
    	
    	switch (cmd) 
    	{        
    		case RTC_TIME_GET:
		        rtc_get(&tm);
		        return copy_to_user((void *)argp, &tm, sizeof(tm)) ? -EFAULT : 0;

    		case RTC_TIME_SET:
		        if (copy_from_user(&tm, (struct rtc_time_t *)argp, sizeof(tm))) 
		        	return -EFAULT;			      	       
		        return rtc_set(tm);

    		default:
			return -EINVAL;
	}
}


#ifdef CONFIG_PROC_FS
	static int rtc_read_proc(char *page, char **start, off_t off,
                       int count, int *eof, void *date)
	{
		char *p = page;
		int len;
		rtc_time_t tm;
		char day[10];
		
		switch(tm.weekday)
		{
			case 0:
				strcpy(day, "Sunday");
			case 1:
				strcpy(day, "Monday");	
			case 2:
				strcpy(day, "Tuesday");	
			case 3:
				strcpy(day, "Wednesday");
			case 4:
				strcpy(day, "Thursday");	
			case 5:
				strcpy(day, "Friday");	
			case 6:
				strcpy(day, "Saturday");
		}
		
		rtc_get(&tm);
  	p += sprintf(
    			  p, "rtc_time: %02d:%02d:%02d\n"
		     		"rtc_date: %04d-%02d-%02d\n",
		     		tm.hour, tm.minute, tm.second,
		     		tm.year, tm.month, tm.date
		     	     );
		p +=sprintf(p, "DAY     : %s\n", day);
		
			
	len = (p - page) - off;
    		
	if (len < 0)
		len = 0;
    		
	*eof = (len <= count) ? 1 : 0;
	*start = page + off;
	return len;
}
#endif

/* open device*/
static int rtc_open(struct inode *inode, struct file *file)
{    	
	spin_lock_init(&rtc_lock);
	spin_lock_irq(&rtc_lock);
	if (rtc_status) 
	{
		spin_unlock_irq(&rtc_lock);
		return -EBUSY;
	}
	/* set dev is busy */
	rtc_status = 1;
	spin_unlock_irq(&rtc_lock);
	return 0;
}


static int rtc_release(struct inode *inode, struct file *file)
{
	rtc_status = 0;
	return 0;
}


static struct file_operations rtc_fops = 
{
	owner:		THIS_MODULE,
	ioctl:		rtc_ioctl,
	open:		  rtc_open,
	release:	rtc_release,
};


static struct miscdevice rtc_dev=
{
	RTC_MINOR,
	"rtc_ds1339",
	&rtc_fops
};


/*
 *	int rtc_init(void)
 *	default time is 2000.1.1.0.0.0
 */
static int __init rtc_init(void)
{
	int ret;   
	unsigned char regvalue;
		    	
	ret = misc_register(&rtc_dev);
	if (0 != ret)
	{
		printk("rtc_ds1307 device register failed!\n");
		return -EFAULT;
	}
   	
    	
	#ifdef CONFIG_PROC_FS
    		create_proc_read_entry("driver/rtc", 0, 0, rtc_read_proc, NULL);
	#endif
 	     
    regvalue = gpio_i2c_read(I2C_DS1339,0x00);
    if(regvalue == 0xff)
    {
        printk("rtc_ds1339 driver init fail for device init error!\n");
	#ifdef CONFIG_PROC_FS
    	remove_proc_entry("driver/rtc", NULL);
	#endif
    	misc_deregister(&rtc_dev);
    	return -1;        
    }
    regvalue &= 0x7f;
    gpio_i2c_write(I2C_DS1339,0x00, regvalue);
    /* set 24 hour mode*/
    regvalue = gpio_i2c_read(I2C_DS1339,0x02);
    regvalue &= 0xbf;
    gpio_i2c_write(I2C_DS1339,0x02, regvalue);
    printk("rtc_ds1339 driver init successful!\n");
	return 0;
}


static void __exit rtc_exit(void)
{    		
	
	#ifdef CONFIG_PROC_FS
    	remove_proc_entry("driver/rtc", NULL);
	#endif
	
	misc_deregister(&rtc_dev);
}


module_init(rtc_init);
module_exit(rtc_exit);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");

