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


//前提，gpio复用关系已经搞定；
#define  SHAKE_PROOF_TIME 5 //单位ms
#define  TIMER_EXPIRES_TIME 30 //单位ms
#define  SAMPLE_INTERVAL 16      //8 circle
#define  SAMPLE_KEEP 2      //2 circle

#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

#define MAX_BUF_NUM 8
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


static void  _74ls165_handler(unsigned long l)
{
	static int times = 0;
	static int circle = 0;
	static int readtimes = 0;
	static unsigned char value = 0x0;
	
    //printk("jiffies:%x\n",jiffies);
    //enable 1-7,to select the first chip
	g_timer.expires = jiffies + 40;
	add_timer(&g_timer);

	gpio_dir_set_bit(0,2,0);
	gpio_dir_set_bit(0,0,1);
	gpio_dir_set_bit(0,1,1);
	
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
			readtimes -- ;
		}
		else
		{
			if(value != 0)
			{
				if(!IS_BUF_FULL(&g_value_buf))
				{
					g_value_buf.value[g_value_buf.write] = value;
					UPDATE_WRITE(&g_value_buf);
				}
			}
			value = 0;
//			printk("g_value:0x%x\n",g_value);
		}
		circle++;
	}
	
	g_vlevel = !g_vlevel;
	times ++;

}


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
		if(!IS_BUF_EMPTY(&g_value_buf))
		{
			copy_to_user((void __user *)arg, &g_value_buf.value[g_value_buf.read], sizeof(unsigned char));
			UPDATE_READ(&g_value_buf);
		}
		else
		{
			char tmp = 0;
			copy_to_user((void __user *)arg, &tmp, sizeof(unsigned char));
		}

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




