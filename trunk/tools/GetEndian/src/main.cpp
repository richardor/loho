#include<stdio.h>

/*
大端格式：在这种格式中，字数据的高字节存储在低地址中，而字数据的低字节则存放在高地址中
小端格式：与大端存储格式相反，在小端存储格式中，低地址中存放的是字数据的低字节，高地址存放的是字数据的高字节
返回1 大端格式
返回0 小端格式
*/
unsigned char GetSystemEndian()
{
    int a = 0x1;
    
    return ((*(unsigned char *)&a) == 1)?0:1;
}

unsigned char GetSystemEndian1()
{
		union w
		{
    	int a;
    	char b;
		}c;
		c.a = 1;
		return (c.b == 1)?0:1;
	
}

int main()
{
  printf("Get the System endian\n");
  unsigned char a = 0;
  a = GetSystemEndian();
  printf("11111111111a(%d)[0 Small endian 1 : Big endian]\n");
  a = GetSystemEndian1();
  printf("22222222222a(%d)[0 Small endian 1 : Big endian]\n");
  return 0;
}

