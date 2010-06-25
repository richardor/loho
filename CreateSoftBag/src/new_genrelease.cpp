#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/*注意:由于时间紧迫，该软件没有考虑变量字节序的问题，这也是基于当前平台和X86字节序
  相同的理由*/
unsigned long Crc32Table[256] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

unsigned long GenerateCrc(unsigned long crc, const char *pBuffer, unsigned long len)
{
	const char *s = (const char *)pBuffer;

	while(len)
	{
		crc = Crc32Table[(crc ^ *s++) & 0xff] ^ (crc >> 8);
		len --;
	}
    return crc;
}

#define UPGRADE_PACKET_MAGIC	0x20101816


#define SOFTWARE_VERSION_LEN			7
#define SOFTWARE_DESCRIPTION_LEN		31


typedef struct _upgradesoftwareentry_{
    char description[SOFTWARE_DESCRIPTION_LEN];
    	/*这里定义的版本号长度是包括字符串结束标志的，
	 下面将加上这个字符串结束标志，同时4字节对齐*/
	char version[(SOFTWARE_VERSION_LEN + 1 +  3) / 4 * 4];
    unsigned long SoftwareId;   /*确定该升级软件的机型，类型（uboot,rootfs,kernel,app）*/
    unsigned long start;        /**/
    unsigned long len;          
}upgradesoftwareentry_t;



#define MAX_SOFTWARE_NUM 10

typedef struct _upgradefileheader_{
    unsigned long magic;/*结构体幻数*/
	unsigned long crc;  /*这里的CRC值是该结构体的下面的域及程序数据的CRC校验值*/
    unsigned long version;/*结构体版本*/
    unsigned long SoftwareNumber;
    upgradesoftwareentry_t software[MAX_SOFTWARE_NUM];
//    upgradesoftwareentry_t software_0;
//    upgradesoftwareentry_t software_1;
//    upgradesoftwareentry_t software_2;
//    upgradesoftwareentry_t software_3;
}upgradefileheader_t;


void usage()
{
    printf("usage:./genrelease device_type_name detail_information customer_information language_information base_version date_information serial_number\n");
}

bool ValidSoftwareVersion(char *pVer)
{
	if(pVer && (strlen(pVer) == SOFTWARE_VERSION_LEN))
	{
		if((pVer[0] == 'T') || (pVer[0] == 'V'))
		{
			for(int i = 1 ; i < SOFTWARE_VERSION_LEN; i ++)
			{
				if(!((pVer[i] >= '0') && (pVer[i] <= '9')))
				{
					return false;
				}
			}
			return true;
		}
	}	
	return false;
}


#define DEVICE_TYPE_NUMBERS 5  

char *device_type_name[] = {
    "RM8304",
    "RM8308", 
    "RM8316",
    "RM8016",
		"RM8504",
		"RM8508",
		"RM8516",
    "X11",
};


typedef enum _device_type_{
    RM8304_T = 0,
    RM8308_T,    
    RM8316_T,
    RM8016_T,
		RM8504_T,
		RM8508_T,
		RM8516_T,
    X11_T,
    NOTYPE_DEFINED_T,
}device_type_t;


char *soft_description_830x[] = {
    "uboot",
    "kernel",
    "rootfs",
		"application"
};


char *soft_description_8016[] = {
    "master uboot",
    "slave uboot",
    "master kernel",
    "slave kernel",
    "master rootfs",
    "slave rootfs",
    "application",
};


#if 0

#define MASTER_UBOOT_ID 0x0101
#define SLAVE_UBOOT_ID  0X0102

#define MASTER_KERNEL_ID 0x0201
#define SLAVE_KERNEL_ID  0X0202

#define MASTER_ROOTFS_ID 0x0301
#define SLAVE_ROOTFS_ID  0x0302

#define APPLICATION_ID   0x0401
#endif
enum software_id{
    MASTER_UBOOT_ID,
    SLAVE_UBOOT_ID,
    MASTER_KERNEL_ID,
    SLAVE_KERNEL_ID,
    MASTER_ROOTFS_ID,
    SLAVE_ROOTFS_ID,
    APPLICATION_ID,
};



device_type_t g_device_type = NOTYPE_DEFINED_T;

bool CheckDeviceType(char *input_device_type)
{
    if(!input_device_type)
    {
        return false;
    }

    char **temp_string = NULL;
    int i = 0;

    temp_string = device_type_name;

    for(i = 0; i < DEVICE_TYPE_NUMBERS; i++)
    {
        if(strcmp(temp_string[i], input_device_type) == 0)
        {
            break;
        }
    }
    if(i < DEVICE_TYPE_NUMBERS)
    {
        g_device_type = (device_type_t)i;
        return true;
    }
    return false;

}


bool CheckDetailInformation(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }
    return true;
}
bool CheckCustomerInformation(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }
    return true;
}


bool CheckLanguageParameter(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }
    return true;
}


bool CheckBaseVersionParameter(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }

    return true;
}


bool CheckDateParameter(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }
    return true;
}

bool CheckSerialNumberParameter(char *argv_string)
{
    if(!argv_string)
    {
        return false;
    }

    return true;
}



int main(int argc, char *argv[])
{
    /*1 输入参数校验*/
    if(argc < 8)
    {
        printf("error !\n");
        return -1;
    }

    if(!CheckDeviceType(argv[1]))
    {
        printf("please input a valid device type name!\n");
        return -1;
    }
    if(!CheckDetailInformation(argv[2]))
    {
        printf("please input a valid detail_information!\n");
        return -1;
    }
    if(!CheckCustomerInformation(argv[3]))
    {
        printf("please input a valid customer_information such as 01 or 02!\n");
        return -1;
    }
    if(!CheckLanguageParameter(argv[4]))
    {
        return -1;
    }
    if(!CheckBaseVersionParameter(argv[5]))
    {
        return -1;
    }
    if(!CheckDateParameter(argv[6]))
    {
        return -1;
    }
    if(!CheckSerialNumberParameter(argv[7]))
    {
        return -1;
    }
    int ret = 0;

    /*2 生成软件升级包的文件头*/
	upgradefileheader_t header;
	header.magic = UPGRADE_PACKET_MAGIC;
	header.version = 0x01;
	header.crc = 0;

    int i = 0;
    int software_fd = -1;
	char *pBuffer = NULL;
	pBuffer = new char[64 * 1024];

  unsigned long *pStart = NULL, *pLen = NULL;
	int ReleaseFd = -1, ReadLen = -1;
	int software_numbers = 0;


    /*不同的平台需要打包的升级软件文件个数不同*/
    switch(g_device_type)
    {
        case RM8304_T:
        case RM8308_T:
        case RM8316_T:
            break;
        case RM8016_T:
        		break;
				case RM8504_T:
				case RM8508_T:
						/*8504 8508 的升级包所包含的文件的个数为了4个 uboot kernel rootfs app
						  boot版本号[8] 文件名[9]
						  内核版本号[10] 文件名[11]
						  rootfs版本号[12] 文件名[13]
						  app 版本号[14] 文件名[15]
						*/
						if((argc != 16) ||\
								(ValidSoftwareVersion(argv[8]) == false)||\
								(ValidSoftwareVersion(argv[10]) == false)||\
								(ValidSoftwareVersion(argv[12]) == false)||\
								(ValidSoftwareVersion(argv[14]) == false))
							{
								printf("8504 8508 error :please input 16 parameters and valid software version!\n");
							  ret = -1;
                goto GENRELEASE_OVER;
							}
							header.SoftwareNumber = 4;
         		  /*boot*/
            	header.software[0].SoftwareId = MASTER_UBOOT_ID;
            	strcpy(header.software[0].version, argv[8]);   
           		printf("master_boot version(%s)\n", argv[8]);
            	/*kernel*/
           	  header.software[1].SoftwareId = MASTER_KERNEL_ID;
          	  strcpy(header.software[1].version, argv[10]);
           	  printf("master_kernel version(%s)\n", argv[10]);
           		/*rootfs*/
            	header.software[2].SoftwareId = MASTER_ROOTFS_ID;
            	strcpy(header.software[2].version, argv[12]);  
            	printf("rootfs version(%s)\n", argv[12]);
            	/*app */
            	header.software[3].SoftwareId = APPLICATION_ID;
            	strcpy(header.software[3].version, argv[14]);
            	printf("app version(%s)\n", argv[14]);
           		
           	  /*set the soft description*/
            	for(i = 0; i < 4; i++)
            	{
                strncpy(header.software[i].description, soft_description_830x[i], SOFTWARE_DESCRIPTION_LEN);    
            	}		
           		
            	/* get the software start point and the len of the software!*/
            	for(i = 0; i < 4;i++)
            	{   
             	   printf("open i(%d), filename(%s)\n",i, argv[9 + i * 2]);
                
        				 software_fd= open(argv[9 + i * 2], O_RDONLY);
        				 if(software_fd < 0)
        				 {
        						printf("open %s file failed\n", argv[9 + i * 2]);
                    ret = -1;
                    goto GENRELEASE_OVER;
        				 }
                
                pLen = &header.software[i].len;
                
                *pLen = 0;
                
        				while(1)
        				{
        					ReadLen = read(software_fd, pBuffer, 64 * 1024);
        					if(ReadLen < 0)
        					{
        						printf("read %s file failed\n", argv[9 + i * 2]);
                        ret = -1;
        						close(software_fd);
                    goto GENRELEASE_OVER;
        					}
        					else if(ReadLen == 0)
        					{
        						close(software_fd);
        						break;
        					}
        					header.crc = GenerateCrc(header.crc, pBuffer, ReadLen);
        					*pLen += ReadLen;
        				}
        				if(*pLen == 0)
        				{
        					printf("%s file is a blank file\n", argv[9 + i * 2]);
        					exit(1);
        				}
            	}
    
        			/*生成文件头*/
            header.software[0].start = sizeof(upgradefileheader_t);
            
        	  header.crc = GenerateCrc(header.crc, (const char *)&header.version, 4);
            for(i = 0; i < 4;i++)
            {
                if(0 == i)
                {   
                    header.software[i].start = sizeof(upgradefileheader_t);
                }
                else
                {
                    header.software[i].start = header.software[i - 1].start + header.software[i - 1].len;
                }
                printf("i(%d), header.software[i].start(%d), header.software[i].len(%d)\n",i, header.software[i].start, header.software[i].len);
            		header.crc = GenerateCrc(header.crc, (const char *)&header.software[i], sizeof(upgradesoftwareentry_t));
            }
						break;

        case RM8516_T:		
            /*8516 的升级包含的升级文件的个数为7个。主从boot,   主从内核，主从rootfs , 主应用程序。*/
            /*主boot版本号 [8]， 文件名[9]，从boot版本号[10]，文件名[11]，
             主内核版本号  [12]，文件名[13]，从内核版本号[14]，文件名[15]，
             主rootfs版本号[16]，文件名[17]，从rootfs版本号[18]，文件名[19]
             应用程序版本号[20]，文件名[21]*/
            if((argc != 22) || (ValidSoftwareVersion(argv[8]) == false) ||\
                (ValidSoftwareVersion(argv[10]) == false) ||\
                (ValidSoftwareVersion(argv[12]) == false) ||\
                (ValidSoftwareVersion(argv[14]) == false) ||\
                (ValidSoftwareVersion(argv[16]) == false) ||\
                (ValidSoftwareVersion(argv[18]) == false) ||\
                (ValidSoftwareVersion(argv[20]) == false))
            {
                printf("8516 error!\n ");  
                ret = -1;
                goto GENRELEASE_OVER;
            }
            
            header.SoftwareNumber = 7;

            /*master boot*/
            header.software[0].SoftwareId = MASTER_UBOOT_ID;
            strcpy(header.software[0].version, argv[8]);   
            printf("master_boot version(%s)\n", argv[8]);
            /*slave boot*/
            header.software[1].SoftwareId = SLAVE_UBOOT_ID;
            strcpy(header.software[1].version, argv[10]); 
            printf("slave_boot version(%s)\n", argv[10]);

            
            /*master kernel*/
            header.software[2].SoftwareId = MASTER_KERNEL_ID;
            strcpy(header.software[2].version, argv[12]);
            printf("master_kernel version(%s)\n", argv[12]);
            
            /*slave kernel*/
            header.software[3].SoftwareId = SLAVE_KERNEL_ID;
            strcpy(header.software[3].version, argv[14]);   
            printf("slave_kernel version(%s)\n", argv[14]);
            
            
            /*master rootfs*/
            header.software[4].SoftwareId = MASTER_ROOTFS_ID;
            strcpy(header.software[4].version, argv[16]);  
            printf("master rootfs version(%s)\n", argv[16]);
            
            
            /*slave rootfs*/
            header.software[5].SoftwareId = SLAVE_ROOTFS_ID;
            strcpy(header.software[5].version, argv[18]); 
            printf("slave rootfs version(%s)\n", argv[18]);
            
            
            
            /*app */
            header.software[6].SoftwareId = APPLICATION_ID;
            strcpy(header.software[6].version, argv[20]);
            printf("app version(%s)\n", argv[20]);
            
            /*set the soft description*/
            for(i = 0; i < 7; i++)
            {
                strncpy(header.software[i].description, soft_description_8016[i], SOFTWARE_DESCRIPTION_LEN);    
            }

            /* get the software start point and the len of the software!*/
            for(i = 0; i < 7;i++)
            {   
                printf("open i(%d), filename(%s)\n",i, argv[9 + i * 2]);
                
        		software_fd= open(argv[9 + i * 2], O_RDONLY);
        		if(software_fd < 0)
        		{
        			printf("open %s file failed\n", argv[9 + i * 2]);
                    ret = -1;
                    goto GENRELEASE_OVER;
        		}
                
                pLen = &header.software[i].len;
                
                *pLen = 0;
                
        		while(1)
        		{
        			ReadLen = read(software_fd, pBuffer, 64 * 1024);
        			if(ReadLen < 0)
        			{
        				printf("read %s file failed\n", argv[9 + i * 2]);
                        ret = -1;
        				close(software_fd);
                        goto GENRELEASE_OVER;
        			}
        			else if(ReadLen == 0)
        			{
        				close(software_fd);
        				break;
        			}
        			header.crc = GenerateCrc(header.crc, pBuffer, ReadLen);
        			*pLen += ReadLen;
        		}
        		if(*pLen == 0)
        		{
        			printf("%s file is a blank file\n", argv[9 + i * 2]);
        			exit(1);
        		}

            }
    
        	/*生成文件头*/
            header.software[0].start = sizeof(upgradefileheader_t);
            
        	header.crc = GenerateCrc(header.crc, (const char *)&header.version, 4);
            for(i = 0; i < 7;i++)
            {
                if(0 == i)
                {   
                    header.software[i].start = sizeof(upgradefileheader_t);
                }
                else
                {
                    header.software[i].start = header.software[i - 1].start + header.software[i - 1].len;
                }
                printf("i(%d), header.software[i].start(%d), header.software[i].len(%d)\n",i, header.software[i].start, header.software[i].len);
            	header.crc = GenerateCrc(header.crc, (const char *)&header.software[i], sizeof(upgradesoftwareentry_t));
            }
            break;
        case X11_T:
            break;
        default:
            printf("can't go to here!\n");
            return -1;
            break;
    }

    /*3 创建软件升级文件名*/
	char ReleaseFileName[128];
    /*升级文件名的组成 设备名称[1]-分类信息[2]-客户信息[3]-语言信息[4]-基线版本号[5]-时间信息[6]-编号[7] */
    sprintf(ReleaseFileName, "%s-%s-%s-%s-%s-%s-%s", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
    
	unlink(ReleaseFileName);

    ReleaseFd = open(ReleaseFileName, O_WRONLY | O_CREAT);
    if(ReleaseFd < 0)
    {
        printf("open %s file failed\n", ReleaseFileName);
        ret = -1;
        goto GENRELEASE_OVER;
    }
    /*4 写入文件头信息*/
    write(ReleaseFd, &header, sizeof(upgradefileheader_t));
    
    /*5 写入需升级文件的内容*/
    switch(g_device_type)
    {
        case RM8304_T:
        case RM8308_T:
        case RM8316_T:
        	  printf("Don't support this machine type!\n");
        	  goto GENRELEASE_OVER;
            break;
        case RM8504_T:
        case RM8508_T:
        		 software_numbers = 4;   
        		 break;
        case RM8016_T:
        case RM8516_T:	
        		 software_numbers = 7;
#if 0        		 
        	for(i = 0; i < 7; i ++)
        	{
        		software_fd = open(argv[9 + i * 2], O_RDONLY);
        		if(software_fd < 0)
        		{
        			printf("open %s file failed\n", argv[9 + i * 2]);
        			unlink(ReleaseFileName);
                    ret = -1;
                    goto GENRELEASE_OVER;
        		}
        		while(1)
        		{
        			ReadLen = read(software_fd, pBuffer, 64 * 1024);
        			if(ReadLen < 0)
        			{
        				printf("read %s file failed\n", argv[9 + i * 2]);
        				unlink(ReleaseFileName);
                        ret = -1;
                        goto GENRELEASE_OVER;
        			}
        			else if(ReadLen == 0)
        			{
        				close(software_fd);
        				break;
        			}
        			write(ReleaseFd, pBuffer, ReadLen);
        		}
        	}
#endif        	
          break;
        default:
            break;
    }
          for(i = 0; i < software_numbers; i ++)
        	{
        		software_fd = open(argv[9 + i * 2], O_RDONLY);
        		if(software_fd < 0)
        		{
        			printf("open %s file failed\n", argv[9 + i * 2]);
        			unlink(ReleaseFileName);
                    ret = -1;
                    goto GENRELEASE_OVER;
        		}
        		while(1)
        		{
        			ReadLen = read(software_fd, pBuffer, 64 * 1024);
        			if(ReadLen < 0)
        			{
        				printf("read %s file failed\n", argv[9 + i * 2]);
        				unlink(ReleaseFileName);
                        ret = -1;
                        goto GENRELEASE_OVER;
        			}
        			else if(ReadLen == 0)
        			{
        				close(software_fd);
        				break;
        			}
        			write(ReleaseFd, pBuffer, ReadLen);
        		}
        	}
    
    /*6 清理，关闭打开的文件*/
	fsync(ReleaseFd);
	fchmod(ReleaseFd, 0777);
	
GENRELEASE_OVER:
		
    if(software_fd >= 0)
    {
        close(software_fd);
    }
    if(ReleaseFd >= 0)
    {
        close(ReleaseFd);
    }
    if(pBuffer != NULL)
    {
			delete []pBuffer;
      pBuffer = NULL;
    }
    return -1;
}


