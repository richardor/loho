#include <linux/module.h>
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
#include <asm/hardware.h>
#include "74hc165.h"


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
#define _74HC165_GPIO	((unsigned int)(IO_ADDRESS(0x20150000)))

#define GPIO_SPACE_SIZE    0x10000
#define GPIO_DIR           0x0400 /* gpio direction register */
#define DIRECTION_OUTPUT   1
#define DIRECTION_INPUT    0

static void gpio_dir_set_bit(unsigned char bGpioPathNum, unsigned char bBitx, unsigned char bDirBit)
{
	int reg = readl(_74HC165_GPIO + bGpioPathNum * GPIO_SPACE_SIZE + GPIO_DIR);

	if(bDirBit == DIRECTION_OUTPUT){
		reg |= (DIRECTION_OUTPUT << bBitx);
		writel(reg, (_74HC165_GPIO + bGpioPathNum * GPIO_SPACE_SIZE + GPIO_DIR));
	}
	else{
		reg &= (~(1 << bBitx));
		writel(reg, (_74HC165_GPIO+(bGpioPathNum) * GPIO_SPACE_SIZE + GPIO_DIR));
	}
}

static void gpio_write_bit(unsigned char bGpioPathNum ,unsigned char bBitx ,unsigned char bBitValue)
{
	if (bBitValue == 1)
		writel((1<<(bBitx)), (_74HC165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx)));
	else if(bBitValue == 0)
		writel(0, (_74HC165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx)));
}

static unsigned char gpio_read_bit(unsigned char bGpioPathNum ,unsigned char bBitx)
{
	char tmp = readl(_74HC165_GPIO + (bGpioPathNum) * GPIO_SPACE_SIZE + (4<<bBitx));
	return (tmp>>bBitx);
}

/*  write by guixing 091110  */
//#define GPIO_74HC165_GROUP1 	1
//#define GPIO_74HC165_GROUP3 	3
//#define GPIO_74HC165_GROUP7 	7
//#define GPIO_74HC165_DATA1	    3   //GPIO7_3
//#define GPIO_74HC165_DATA2	    2   //GPIO1_2
//#define GPIO_74HC165_SHLD	    2   //GPIO3_2

//#define GPIO_74HC165_CLOCK	    3   //GPIO3_3

//modify by lyh 2010.03.02	74hc165到GPIO管脚改变
#define GPIO_74HC165_GROUP2 	2
#define GPIO_74HC165_DATA    	2   //GPIO2_2
#define GPIO_74HC165_CLOCK	   	3   //GPIO2_3
#define GPIO_74HC165_SHLD	    	4   //GPIO2_4

//add by lyh 2010.03.02
#define IOCONFIG(x)  (IO_ADDRESS(0x200f0000+0x4*(x)))
#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

unsigned int IoState = 0;

/*  write by guixing 091110  */
static void  _74hc165_handler(unsigned long l)
{
	static int stage = 0;/*采样期*/
	static int ShiftBit = 0;/*移位到那一个BIT位了*/
	static unsigned char RegisterValue1 = 0;
	static unsigned char RegisterValue2 = 0;
	
	g_timer.expires = jiffies + 4;
	add_timer(&g_timer);

	if(stage)
	{
		/*移位期间，移位引脚须处于高电平，在时钟的下降沿移出一个BIT位*/		
		gpio_write_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_SHLD, 1);/*拉高移位引脚*/
		if((ShiftBit % 2) == 0)/*下降沿触发，拉低时钟引脚，此时不读数据，等待数据稳定*/
		{			
			gpio_write_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_CLOCK, 0);
		}
		else
		{
			/*先移的是最高位*/
			RegisterValue1 |= ((gpio_read_bit(GPIO_74HC165_GROUP2, 
					GPIO_74HC165_DATA) & 0x01) << (7 - ShiftBit / 2));

		/*	RegisterValue2 |= ((gpio_read_bit(GPIO_74HC165_GROUP1, 
					GPIO_74HC165_DATA2) & 0x01) << (7 - ShiftBit / 2));			
		*/
			/*拉高时钟引脚*/
			gpio_write_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_CLOCK, 1);
			if(ShiftBit >= 15)/*一次读寄存器完成*/
			{
				stage = 0;
				//IoState = RegisterValue2;
				//IoState = (IoState << 8) | RegisterValue1;
				IoState = RegisterValue1;
#if	0		
				printk("74LS165 RegisterValue1:1--(%d)(%d)(%d)(%d) 5--(%d)(%d)(%d)(%d)\n", 
					RegisterValue1 & 0x01,
					(RegisterValue1 >> 1) & 0x01, 
					(RegisterValue1 >> 2) & 0x01,
					(RegisterValue1 >> 3) & 0x01,
					(RegisterValue1 >> 4) & 0x01,
					(RegisterValue1 >> 5) & 0x01,
					(RegisterValue1 >> 6) & 0x01,
					(RegisterValue1 >> 7) & 0x01);
				
				printk("\t\t\t\t74LS165 RegisterValue2:1>>(%d)(%d)(%d)(%d)  5--(%d)(%d)(%d)(%d)\n\n\n", 
					RegisterValue2 & 0x01,
					(RegisterValue2 >> 1) & 0x01, 
					(RegisterValue2 >> 2) & 0x01,
					(RegisterValue2 >> 3) & 0x01,
					(RegisterValue2 >> 4) & 0x01,
					(RegisterValue2 >> 5) & 0x01,
					(RegisterValue2 >> 6) & 0x01,
					(RegisterValue2 >> 7) & 0x01);	
#endif
			}
		}
		ShiftBit ++;
	}
	else
	{
		/*设置控制GPIO口的方向*/
		gpio_dir_set_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_DATA, DIRECTION_INPUT);/*数据DATA1，输入*/
        	//gpio_dir_set_bit(GPIO_74HC165_GROUP1, GPIO_74HC165_DATA2, DIRECTION_INPUT);/*数据DATA2，输入*/
		gpio_dir_set_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_CLOCK, DIRECTION_OUTPUT);/*时钟CLOCK，输出*/
		gpio_dir_set_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_SHLD, DIRECTION_OUTPUT);/*移位SH/LD，输出*/
		/*采样期，拉低移位引脚*/
		gpio_write_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_SHLD, 0);
		gpio_write_bit(GPIO_74HC165_GROUP2, GPIO_74HC165_CLOCK, 1);
		stage = 1;/*下面开始移位了*/
		ShiftBit = 0;
		RegisterValue1 = 0;
		RegisterValue2 = 0;
	}
}

static int _74hc165_open(struct inode *inode, struct file *filp)
{
    return 0;		
}

static int _74hc165_release(struct inode *inode, struct file *filp)
{
    return 0;	
}


static int _74hc165_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;	
    switch(cmd)
    {
	case _IOC_GET_VALUE:
		ret = copy_to_user((void __user *)arg, &IoState, sizeof(unsigned int));
	    break; 
	default:
	    return -1;           
    }
    return 0;
}


static struct file_operations _74hc165_fops = {
	  owner:THIS_MODULE,
      open:_74hc165_open,
      ioctl:_74hc165_ioctl,
      release:_74hc165_release,
};

static struct miscdevice key_gpio_dev = {
    MISC_DYNAMIC_MINOR,
    "74hc165",
    &_74hc165_fops,
};



//gpio的复用关系要放在uboot下面；
static int __init _74hc165_init(void)
{
    int  ret=0;

	//add by lyh 2010.03.02 更改GPIO复用关系为IO输入
	WRITE_REG(IOCONFIG(31), 0);
	WRITE_REG(IOCONFIG(32), 0);
	WRITE_REG(IOCONFIG(33), 0);
	
	init_waitqueue_head(&read_wait);

	memset(&g_value_buf,0,sizeof(g_value_buf));

	init_timer(&g_timer);
	g_timer.function = _74hc165_handler;
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


static void __exit _74hc165_exit(void)
{
	del_timer(&g_timer);
    misc_deregister(&key_gpio_dev);
}

module_init(_74hc165_init);
module_exit(_74hc165_exit);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Real Time Clock interface for HI3511");




