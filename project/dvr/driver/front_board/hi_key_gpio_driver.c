#include <linux/module.h>
//#include <linux/config.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>
// #include <linux/kcom.h>
#include <asm/hardware.h>
#include <linux/interrupt.h>

#include "hi_key_gpio_driver.h"

#define SYS_CONTROL_REG		0x20050000
#define TIMER_GROUP2_REG    0x20010000
#define IOCONFIGREG(x)  (IO_ADDRESS(0x200f0000+0x4*(x))) //Ziven@100224 3515 ds page 92

#define TIMER_GROUP2_IRQ_NUM    5
#define TIMER_LOAD_OFFSET (timer_reg_value+ 0)
#define TIMER_VALUE_OFFSET  (timer_reg_value+ 4)
#define TIMER_CONTROL_OFFSET  (timer_reg_value+ 8)
#define TIMER_INTCLR_OFFSET (timer_reg_value+ 0xc) 
#define TIMER_RIS_OFFSET  (timer_reg_value+ 0x10)
#define TIMER_MIS_OFFSET  (timer_reg_value+ 0x14)
#define TIMER_BGLOAD_OFFSET  (timer_reg_value+ 0x18)


//前提，gpio复用关系已经搞定；
#define  SHAKE_PROOF_TIME 5 //单位ms
#define  TIMER_EXPIRES_TIME 30 //单位ms
#define  REPEAT_KEY_TIMEOUT 300 //单位ms

#define  GPIO_0_BASE_ADDR  0x20150000
#define  GPIO_2_BASE_ADDR  0x20170000
#define  GPIO_3_BASE_ADDR  0x20180000
#define  GPIO_5_BASE_ADDR  0x201a0000
#define  GPIO_0_DIR_OFFSET   (GPIO_0_BASE_ADDR_VIRTUAL+0x400)
#define  GPIO_0_INTR_MASK  (GPIO_0_BASE_ADDR_VIRTUAL+0x410)
#define  GPIO_0_READ_DATA   (GPIO_0_BASE_ADDR_VIRTUAL+0x3e4)
#define  GPIO_2_DIR_OFFSET   (GPIO_2_BASE_ADDR_VIRTUAL+0x400)
#define  GPIO_2_INTR_MASK  (GPIO_2_BASE_ADDR_VIRTUAL+0x410)
#define  GPIO_2_READ_DATA   (GPIO_2_BASE_ADDR_VIRTUAL+0x1e0)
#define  GPIO_5_DIR_OFFSET   (GPIO_5_BASE_ADDR_VIRTUAL+0x400)
#define  GPIO_5_INTR_MASK  (GPIO_5_BASE_ADDR_VIRTUAL+0x410)
#define  GPIO_5_READ_DATA   (GPIO_5_BASE_ADDR_VIRTUAL+0x07c) 
//#define  GPIO_5_WRITE_DATA  (GPIO_5_BASE_ADDR_VIRTUAL+0x3fc)

#define GPIO_DIR           0x0400 /* gpio direction register */
#define OUTPUT   1
#define INPUT    0

#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

unsigned char  key_value[256];
unsigned int head=0;
unsigned int tail=0;
unsigned int time_acculte1=0;
unsigned int enable_repeatkey=0;
unsigned int key_has_release1=1;
unsigned int key_has_release2=1;
unsigned int time_acculte2=0;
unsigned int repkey_timeout=0;
unsigned int GPIO_0_BASE_ADDR_VIRTUAL=0;
unsigned int GPIO_2_BASE_ADDR_VIRTUAL=0;
unsigned int GPIO_5_BASE_ADDR_VIRTUAL=0;
unsigned int timer_reg_value=0;
// unsigned int sys_reg_value=0;

static void gpio_dir_set_bit(unsigned int  iBAddr, unsigned char bBitx, unsigned char bDirBit)
{
	int reg = readl(((unsigned int)(IO_ADDRESS(iBAddr))) + GPIO_DIR);
	if(bDirBit == OUTPUT){
		reg |= (OUTPUT << bBitx);
		writel(reg, (((unsigned int)(IO_ADDRESS(iBAddr))) + GPIO_DIR));
	}
	else{
		reg &= (~(1 << bBitx));
		writel(reg, (((unsigned int)(IO_ADDRESS(iBAddr))) + GPIO_DIR));
	}
}

static void gpio_write_bit(unsigned int iBAddr ,unsigned char bBitx ,unsigned char bBitValue)
{
	if (bBitValue == 1)
		writel((1<<(bBitx)), (((unsigned int)(IO_ADDRESS(iBAddr))) + (4<<bBitx)));
	else if(bBitValue == 0)
		writel(0, (((unsigned int)(IO_ADDRESS(iBAddr))) + (4<<bBitx)));
}

static unsigned char gpio_read_bit(unsigned int iBAddr ,unsigned char bBitx)
{
	char tmp = readl(((unsigned int)(IO_ADDRESS(iBAddr))) + (4<<bBitx));
	return (tmp>>bBitx);
}
static irqreturn_t  gpio_scankey_handler(int irq, void *dev_id)
{
	unsigned int  reg_tmp=0;
 	static unsigned int perkey = 0;
	unsigned int  reg_tmp1=0;
	unsigned int  reg_tmp2=0;
	static int swresetrepeatcount=0,havevgarepeatcount =0;
	WRITE_REG(TIMER_INTCLR_OFFSET,0x1);
	mdelay(1);
	reg_tmp=READ_REG(GPIO_5_READ_DATA);
	reg_tmp1=gpio_read_bit(GPIO_0_BASE_ADDR,5);///READ_REG(GPIO_2_READ_DATA);vga input signal!!
	reg_tmp2=gpio_read_bit(GPIO_2_BASE_ADDR,5);//READ_REG(GPIO_2_READ_DATA); sw reset button!!
	if (reg_tmp2==0)
		swresetrepeatcount++;
	else
		swresetrepeatcount = 0;

	if (reg_tmp1==0)
		havevgarepeatcount++;
	else
		havevgarepeatcount = 0;
		if ((perkey != reg_tmp))	{
			perkey = reg_tmp;
			time_acculte1 = repkey_timeout +1;
		} else if ((enable_repeatkey!=0)&&((reg_tmp &0x1f)!=0x1f))	{
			time_acculte1+=2*TIMER_EXPIRES_TIME;
		}
	
	if((time_acculte1>repkey_timeout)&&((reg_tmp &0x1f)!=0x1f)){
		reg_tmp = ((~reg_tmp)& 0x1f);
		if (reg_tmp>=0x10) {
			reg_tmp = reg_tmp-0x10;
		}
		key_value[tail]=reg_tmp;          	
		tail++;
		if(tail >200)
		{
			if((tail-head)<255)
			{
				memcpy(&key_value[0],&key_value[head],(tail-head));
				tail=tail-head;
				head=0;
			}
			else
			{
				memcpy(&key_value[0],&key_value[1],254);
				tail=254;
				head=0;
			}
		}
		time_acculte1=0;
	}
	if (swresetrepeatcount >= 75){
		key_value[tail]=16;
		tail++;
		if(tail >200)
		{
			if((tail-head)<255)
			{
				memcpy(&key_value[0],&key_value[head],(tail-head));
				tail=tail-head;
				head=0;
			}
			else
			{
				memcpy(&key_value[0],&key_value[1],254);
				tail=254;
				head=0;
			}
		}
		swresetrepeatcount = 0;		
	}
	if (havevgarepeatcount >= 15)	{
		key_value[tail]=0x20;
		tail++;
		if(tail >200)
		{
			if((tail-head)<255)
			{
				memcpy(&key_value[0],&key_value[head],(tail-head));
				tail=tail-head;
				head=0;
			}
			else
			{
				memcpy(&key_value[0],&key_value[1],254);
				tail=254;
				head=0;
			}
		}
		havevgarepeatcount = 0;
	}
// 	WRITE_REG(GPIO_5_WRITE_DATA,0xff);
    return IRQ_HANDLED;
}


static int gpio_open(struct inode *inode, struct file *filp)
{
    return 0;		
}

static int gpio_release(struct inode *inode, struct file *filp)
{
// 	WRITE_REG(TIMER_INTCLR_OFFSET,0x1);
// 	free_irq(TIMER_GROUP2_IRQ_NUM, 0);
    return 0;	
}

static int gpio_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned char tmp=0xff;
    switch(cmd)
    {
	case KEY_IOC_SET_ENABLE_REPKEY:
	    enable_repeatkey=(unsigned int)arg;
	    break;
	case KEY_IOC_SET_REPKEY_TIMEOUT:
	    if(enable_repeatkey==0)
	    {
		printk("enable repkey not enabled!\n");
		return -1;
	    }	
	    repkey_timeout=(unsigned int)arg;
	    break;
	case KEY_IOC_GET_KEYVALUE:
	    if(head==tail)
	    {
		    if(copy_to_user((void __user *)arg, &tmp, sizeof(unsigned char))) return -EFAULT;
	    }
	    else
	    {
			if(copy_to_user((void __user *)arg, &key_value[head], sizeof(unsigned char))) return -EFAULT;
		head++;
	    }
	    break; 
	default:
	    return -1;           
    }
    return 0;
}


static struct file_operations key_gpio_fops = {
owner:THIS_MODULE,
      open:gpio_open,
      ioctl:gpio_ioctl,
      release:gpio_release,
};

static struct miscdevice key_gpio_dev = {
    MISC_DYNAMIC_MINOR,
    "hi_key_gpio",
    &key_gpio_fops,
};

static void gpio_config(void) {
	int value = 0;
	int i = 0;
	for (i=20;i<25;i++)	{ //GPIO5-0~4 
		value = READ_REG(IOCONFIGREG(i));
		value |= 1;
		WRITE_REG(IOCONFIGREG(i), value);
	}
	value = READ_REG(IOCONFIGREG(8)); //GPIO3-6 HDD_LED
	value |= 1;
	WRITE_REG(IOCONFIGREG(8), value);
	
// 	value = READ_REG(IOCONFIGREG(9)); //GPIO3-7 BUZZER
// 	value |= 1;
// 	WRITE_REG(IOCONFIGREG(9), value);

	WRITE_REG(IOCONFIGREG(34), 0); //GPIO2-5 FOR SWReset

}

//gpio的复用关系要放在uboot下面；
static int __init hi_gpio_init(void)
{
    unsigned char reg_tmp=0;
//     unsigned int reg_timer=0;
    signed int  ret=0;
	gpio_config();
    GPIO_5_BASE_ADDR_VIRTUAL=(unsigned int)IO_ADDRESS(GPIO_5_BASE_ADDR);
    timer_reg_value=(unsigned int )IO_ADDRESS(TIMER_GROUP2_REG);
//     sys_reg_value=(unsigned int )IO_ADDRESS(SYS_CONTROL_REG);
    reg_tmp=(unsigned char)READ_REG(GPIO_5_DIR_OFFSET);
    reg_tmp &= 0xE0;  
    WRITE_REG(GPIO_5_DIR_OFFSET,reg_tmp);
    WRITE_REG(GPIO_5_INTR_MASK,0);   
	gpio_dir_set_bit(GPIO_2_BASE_ADDR,5,INPUT);
	gpio_dir_set_bit(GPIO_0_BASE_ADDR,5,INPUT);
    // set the ref clock, and set timer value
//     reg_timer=READ_REG(sys_reg_value);
//     reg_timer &=0xfff7ffff;
//     WRITE_REG(sys_reg_value,reg_timer);
    //WRITE_REG(sys_reg_value,0x110127); 

    WRITE_REG(TIMER_LOAD_OFFSET,2*3000*TIMER_EXPIRES_TIME); //Ziven@100224 144000--> 220000 -->3000
    WRITE_REG(TIMER_INTCLR_OFFSET,0x1);
    WRITE_REG(TIMER_CONTROL_OFFSET,0xe2);
    
    ret=request_irq(TIMER_GROUP2_IRQ_NUM,&gpio_scankey_handler,0,"timer 2 interrupt",0);
    if(ret)
    {
	printk("timer2 irq request failed!\n");
	return -1;
    }
    // 注册设备；
    ret = misc_register(&key_gpio_dev);
    if (ret)
    {
	printk(KERN_ERR "register misc dev for key gpio fail!\n");
	free_irq(TIMER_GROUP2_IRQ_NUM, 0);
	return ret;
    }
    memset(key_value,0xff,256);
    return 0;     
}


static void __exit hi_gpio_exit(void)
{
// 	WRITE_REG(TIMER_INTCLR_OFFSET,0x1);
	free_irq(TIMER_GROUP2_IRQ_NUM, 0);
    misc_deregister(&key_gpio_dev);
}

module_init(hi_gpio_init);
module_exit(hi_gpio_exit);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Real Time Clock interface for HI3511");




