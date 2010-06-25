#include<stdio.h>
#include"vio.h"
#include"adchip.h"




int main(int argc, char *argv[])
{
    tvsystem_t system = TS_PAL;
    resolution_t resolution = RES_CIF;
    int max_channel_num = 16;
    channelid_t channel[MAX_VIDEO_NUM] = {0};
    int count = 0;
    
    InitPlatform();

    /*1 配置AD chip 的工作模式*/
    ADChipInit(system);
    SetADChipWordMode(VI_WORK_MODE_4D1);

    /*2 */
    VideoInit(system, resolution, max_channel_num);

    DEBUG_INFO(("aaaaaaaaaaaaaaaaaa\n"));
    StartVideoOut();

    
    for(int i = 0; i < MAX_VIDEO_NUM; i++)
    {
        channel[i] = i;
    }
    DEBUG_INFO(("bbbbbbbbbbbbbbbbb\n"));



    while(getchar() != 'q')
    {
        switch(count)
        {
            case 0:
                ChangeVideoOut(OM_ONE, channel);
                break;
            case 1:
                ChangeVideoOut(OM_FOUR, channel);
                break;
            case 2:
                ChangeVideoOut(OM_NINE, channel);
                break;
            case 3:
            default:
                ChangeVideoOut(OM_SIXTEEN, channel);
                break;

        }
        count++;
        if(count == 4)
        {
            count = 0;
        }
    }
    return 0;
}

