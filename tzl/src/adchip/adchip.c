#include"adchip.h"


static bool ad_init_flag = false;
static int adchip_fd = -1;
static tvsystem_t adchip_system = TS_INVALID;



void ADChipInit(tvsystem_t system)
{   
    DEBUG_INFO(("ADChipInit\n"));
    if(ad_init_flag == false)
    {
        adchip_system = system;
        adchip_fd = open(ADCHIP_FILE_A, O_RDWR);
        if(adchip_fd >= 0)
        {
            DEBUG_INFO(("open the adchip dev (%s)success!\n", ADCHIP_FILE_A));
            return;
        }
        ad_init_flag = true;
    }
	return;	
}


void ADChipUninit()
{
    DEBUG_INFO(("ADChipUninit\n"));
    if(ad_init_flag)
    {
        adchip_system = TS_INVALID;
        if(adchip_fd >= 0)
        {
            close(adchip_fd);
            DEBUG_INFO(("close the adchip_fd (%d)\n", adchip_fd));
        }
    }
}


int SetADChipWordMode(VI_WORK_MODE_E enWorkMode)
{
	DEBUG_INFO(("SetADChipWordMode\n"));
    int i;
    int video_mode;
    tw2866_video_norm stVideoMode;
    tw2866_work_mode work_mode;

    video_mode = (TS_PAL == adchip_system) ? TW2866_PAL : TW2866_NTSC;
    
    /*设置AD芯片的工作制式*/
    for (i=0; i<4; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(adchip_fd, TW2866_SET_VIDEO_NORM, &stVideoMode))
        {
            printf("set tw2866(chip:%d) video mode(mode:%d) fail fd:%d\n", i,video_mode,adchip_fd);
            close(adchip_fd);
            return -1;
        }
    }
    /*设置AD芯片的工作模式。TW2866_4D1_MODE*/
    for (i=0; i<4; i++)
    {
        work_mode.chip = i;
        if (VI_WORK_MODE_4D1 == enWorkMode)
        {
            work_mode.mode = TW2866_4D1_MODE;
        }
        else if (VI_WORK_MODE_2D1 == enWorkMode)
        {
            work_mode.mode = TW2866_2D1_MODE;
        }
        else
        {
            printf("work mode not support\n");
            return -1;
        }
		if(ioctl(adchip_fd, TW2866_SET_WORK_MODE, &work_mode))
		{
            printf("TW2866_SET_WORK_MODE err\n");
            close(adchip_fd);
            return -1;
        }
    }
    
    return 0;	
}





