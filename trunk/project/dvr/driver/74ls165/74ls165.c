#include <linux/module.h>
#include <linux/config.h>
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
#include "74ls165.h"


#define D74LS165_BY_XZYANG

//前提，gpio复用关系已经搞定；
#define  SHAKE_PROOF_TIME 5 //单位ms
#define  TIMER_EXPIRES_TIME 30 //单位ms
#define  SAMPLE_INTERVAL 16      //8 circle
#define  SAMPLE_KEEP 2      //2 circle

#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

//#define MAX_BUF_NUM 8
#define MAX_BUF_NUM 2
typedef struct
{
	unsigned char  value[MAX_BUF_NUM];
	unsigned int read;
	unsigned int write;
}VALUE_BUF_S;

unsigned int g_vlevel = 0;
static struct timer_list g_timer;
VALUE_BUF_S g_value_buf;

static inline int IS_BUF_FULL(VALUE_BUF_S *pbuf)
{
	unsigned int read = pbuf->read;
	unsigned int write = pbuf->write;
	return ((read - write == 1)||(read + MAX_BUF_NUM - read == 1));
}

static inline int IS_BUF_EMPTY(VALUE_BUF_S *pbuf)
{
	unsigned int read = pbuf->read;
	unsigned int write = pbuf->write;
	return (read == write);
}

static inline void UPDATE_READ(VALUE_BUF_S *pbuf)
{
	pbuf->read ++;
	if(pbuf->read == MAX_BUF_NUM)
	{
		pbuf->read = 0;
	}
}

static inline void UPDATE_WRITE(VALUE_BUF_S *pbuf)
{
	pbuf->write ++;
	if(pbuf->write == MAX_BUF_NUM)
	{
		pbuf->write = 0;
	}
}

unsigned int timer_reg_value=0;
unsigned int sys_reg_value=0;

static	wait_queue_head_t	read_wait;

/******************************************************************************/
#define _74LS165_GPIO	((unsigned int)(IO_ADDRESS(0x101E6000)))

#define GPIO_SPACE_SIZE    0x1000
#define GPIO_DIR           0x0400 /* gpio direction register */
#define DIRECTION_OUTPUT   1
#define DIRECTION_INPUT    0
#if 0
#define readl(addr) *((volatile unsigned int*)(addr))  
#define writel(b,addr) ((*(volatile unsigned int *) (addr)) = (b))
#endif
static void gpio_dir_set_bit(unsigned char bGpioPathNum, unsigned char bBitx, unsigned char bDirBit)
{
	int reg = readl(_74LS165_GPIO + bGpioPathNum * GPIO_SPACE_SIZE + GPIO_DIR);

	if(bDirBit == DIRECTION_OUTPUT){
		reg |= (DIRECTION_OUTPUT << bBitx);
		writel(reg, (_74LS165_GPIO + bGpioPathNum * GPIO_SPACE_SIZE + GPIO_DIR));
	}
	else{
		reg &= (~(1 << bBitx));
		writel(reg, (_74LS165_GPIO+(bGpioPathNum) * GPIO_SPACE_SIZE + GPIO_DIR));
	}
}

static void gpio_write_bit(unsigned char bGpioPathNum ,unsigned char bBitx ,unsigned char bBitValue)
{
	if (bBitValue == 1)
		writel((1<<(bBitx)), (_74LS165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx)));
	else if(bBitValue == 0)
		writel(0, (_74LS165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx)));
}

static unsigned char gpio_read_bit(unsigned char bGpioPathNum ,unsigned char bBitx)
{
	char tmp = readl(_74LS165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx));
	return (tmp>>bBitx);
}


#if defined(D74LS165_BY_XZYANG)
#define GPIO_74LS165_GROUP 	0
#define GPIO_74LS165_DATA	2
#define GPIO_74LS165_CLOCK	0
#define GPIO_74LS165_SHLD	1
unsigned char IoState = 0x00;
static void  _74ls165_handler(unsigned long l)
{
	static int stage = 0;/*采样期*/
	static int ShiftBit = 0;/*移位到那一个BIT位了*/
	static unsigned char RegisterValue = 0;
	
	g_timer.expires = jiffies + 4;
	add_timer(&g_timer);

	if(stage)
	{
		/*移位期间，移位引脚须处于高电平，在时钟的下降沿移出一个BIT位*/		
		gpio_write_bit(GPIO_74LS165_GROUP, GPIO_74LS165_SHLD, 1);/*拉高移位引脚*/
		if((ShiftBit % 2) == 0)/*下降沿触发，拉低时钟引脚，此时不读数据，等待数据稳定*/
		{			
			gpio_write_bit(GPIO_74LS165_GROUP, GPIO_74LS165_CLOCK, 0);
		}
		else
		{
			/*先移的是最高位*/
#if 0
			RegisterValue |= ((gpio_read_bit(GPIO_74LS165_GROUP, 
					GPIO_74LS165_DATA) & 0x01) << (ShiftBit / 2));
#else
			RegisterValue |= ((gpio_read_bit(GPIO_74LS165_GROUP, 
					GPIO_74LS165_DATA) & 0x01) << (7 - ShiftBit / 2));
#endif
			/*拉高时钟引脚*/
			gpio_write_bit(GPIO_74LS165_GROUP, GPIO_74LS165_CLOCK, 1);
			if(ShiftBit >= 15)/*一次读寄存器完成*/
			{
				stage = 0;
				IoState = RegisterValue;

#if	0		
				printk("74LS165:(1:%d)(2:%d)(3:%d)(4:%d)(5:%d)(6:%d)(7:%d)(8:%d)\n", 
					RegisterValue & 0x01,
					(RegisterValue >> 1) & 0x01, 
					(RegisterValue >> 2) & 0x01,
					(RegisterValue >> 3) & 0x01,
					(RegisterValue >> 4) & 0x01,
					(RegisterValue >> 5) & 0x01,
					(RegisterValue >> 6) & 0x01,
					(RegisterValue >> 7) & 0x01);
#endif
			}
		}
		ShiftBit ++;
	}
	else
	{
		/*设置控制GPIO口的方向*/
		gpio_dir_set_bit(GPIO_74LS165_GROUP, GPIO_74LS165_DATA, 0);/*数据DATA，输入*/
		gpio_dir_set_bit(GPIO_74LS165_GROUP, GPIO_74LS165_CLOCK, 1);/*时钟CLOCK，输出*/
		gpio_dir_set_bit(GPIO_74LS165_GROUP, GPIO_74LS165_SHLD, 1);/*移位SH/LD，输出*/
		/*采样期，拉低移位引脚*/
		gpio_write_bit(GPIO_74LS165_GROUP, GPIO_74LS165_SHLD, 0);
		gpio_write_bit(GPIO_74LS165_GROUP, GPIO_74LS165_CLOCK, 1);
		stage = 1;/*下面开始移位了*/
		ShiftBit = 0;
		RegisterValue = 0;
	}
}
#else
static void  _74ls165_handler(unsigned long l)
{
	static int times = 0;
	static int circle = 0;
	static int readtimes = 0;
	static unsigned char value = 0x0;
	
    //printk("jiffies:%x\n",jiffies);
    //enable 1-7,to select the first chip
	g_timer.expires = jiffies + 4;
	add_timer(&g_timer);

	gpio_dir_set_bit(0,2,0);/*数据DATA*/
	gpio_dir_set_bit(0,0,1);/*时钟CLOCK*/
	gpio_dir_set_bit(0,1,1);/*移位SH/LD*/
	
	/*gpio模拟时钟*/
	gpio_write_bit(0, 0, g_vlevel);
	
	if(times%2 == 0)/*下降触发*/
	{
		/*采样*/
		if((circle%SAMPLE_INTERVAL) == 0 )
		{
			gpio_write_bit(0, 1, 0);
			//mdelay(SHAKE_PROOF_TIME);
		}
		else
		{
			gpio_write_bit(0, 1, 1);
		}
		
		if((circle%SAMPLE_INTERVAL) == 1)
		{
			readtimes = 8;
		}
		
		if(readtimes)
		{
			char tmp = gpio_read_bit(0,2);
			value |= (tmp<<(readtimes-1));
//			printk("bit:%d is :%d\n",readtimes,tmp);
			readtimes -- ;
		}
		else
		{
				if(!IS_BUF_FULL(&g_value_buf))
				{
					g_value_buf.value[g_value_buf.write] = value;
//					printk("value:%2x\n",value);
					UPDATE_WRITE(&g_value_buf);
				wake_up_interruptible(&read_wait);				
				}
//			printk("g_value:0x%x\n",g_value);
		}
		circle++;
	}
	
	g_vlevel = !g_vlevel;
	times ++;

}
#endif

static int _74ls165_open(struct inode *inode, struct file *filp)
{
    return 0;		
}

static int _74ls165_release(struct inode *inode, struct file *filp)
{
    return 0;	
}


static int _74ls165_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
//    unsigned char tmp=0xff;
    switch(cmd)
    {
	case _IOC_GET_VALUE:
#if defined(D74LS165_BY_XZYANG)
		copy_to_user((void __user *)arg, &IoState, sizeof(unsigned char));
#else
		if(wait_event_interruptible(read_wait, !IS_BUF_EMPTY(&g_value_buf)))
		{
			return -ERESTARTSYS;
		}
			
		copy_to_user((void __user *)arg, &g_value_buf.value[g_value_buf.read], sizeof(unsigned char));
		UPDATE_READ(&g_value_buf);
#endif
	    break; 
	default:
	    return -1;           
    }
    return 0;
}


static struct file_operations _74ls165_fops = {
	  owner:THIS_MODULE,
      open:_74ls165_open,
      ioctl:_74ls165_ioctl,
      release:_74ls165_release,
};

static struct miscdevice key_gpio_dev = {
    MISC_DYNAMIC_MINOR,
    "74ls165",
    &_74ls165_fops,
};



//gpio的复用关系要放在uboot下面；
static int __init _74ls165_init(void)
{
    int  ret=0;

//    g_value = 0xff;

#if 0    // set the ref clock, and set timer value
    timer_reg_value=(unsigned int )IO_ADDRESS(TIMER_GROUP2_REG);
    sys_reg_value=(unsigned int )IO_ADDRESS(SYS_CONTROL_REG);

    reg_timer=READ_REG(sys_reg_value);
    reg_timer &=0xfff7ffff;
    //WRITE_REG(sys_reg_value,reg_timer);
    WRITE_REG(sys_reg_value,0x110127);

    WRITE_REG(TIMER_LOAD_OFFSET,2*144000*TIMER_EXPIRES_TIME);
    WRITE_REG(TIMER_INTCLR_OFFSET,0x1);
    WRITE_REG(TIMER_CONTROL_OFFSET,0xe2);
    
    ret = request_irq(TIMER_GROUP2_IRQ_NUM,&_74ls165_handler,SA_INTERRUPT,"timer 2 interrupt",0);
    if(ret)
    {
		printk("timer2 irq request failed!\n");
		return -1;
    }
#endif
	init_waitqueue_head(&read_wait);

	memset(&g_value_buf,0,sizeof(g_value_buf));

	init_timer(&g_timer);
	g_timer.function = _74ls165_handler;
	g_timer.expires = jiffies + 4;
	add_timer(&g_timer);

    // 注册设备；
    ret = misc_register(&key_gpio_dev);
    if (ret)
    {
		printk(KERN_ERR "register misc dev for key gpio fail!\n");
		del_timer(&g_timer);
		return ret;
    }

    return 0;     
}


static void __exit _74ls165_exit(void)
{
	del_timer(&g_timer);
    misc_deregister(&key_gpio_dev);
}

module_init(_74ls165_init);
module_exit(_74ls165_exit);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Real Time Clock interface for HI3511");




