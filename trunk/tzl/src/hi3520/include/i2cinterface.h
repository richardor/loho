#ifndef __I2CINTERFACE_H__
#define __I2CINTERFACE_H__

struct i2cinterface
{
	/*类型，比如有的I2C设备读写某个寄存器直接发送地址
	即可，有的则还需要发送其他信息，比如TW2834还需要发
	送页号才能读取到寄存器的值*/	
	unsigned char type;
	unsigned char i2caddr;/*i2c device address*/
	unsigned char regaddr;/*register address*/
	unsigned char operation;/*read(0) or write(1)*/
	unsigned char value;/*read value or write value*/
	unsigned char reserver[11];
};

#define I2CINTERFACE_CMD_TEMPLATE		0x0a

#endif

