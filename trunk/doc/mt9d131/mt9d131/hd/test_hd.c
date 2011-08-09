#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_vo.h"
#include "hi_comm_vi.h"
#include "hi_comm_vpp.h"
#include "hi_comm_vdec.h"
#include "hi_comm_venc.h"
#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_vpp.h"
#include "mpi_venc.h"
#include "mpi_vdec.h"
#include "mt9d131.h"
#include "adv7179.h"

int IMAGE_SIZE=1600*1200;
int IMAGE_WIDTH=1600;
int IMAGE_HEIGHT=1200;

#define CHECKRET(str,ret)\
    do {\
        if (HI_SUCCESS != ret)\
        {\
            printf("ERROR! "str" <ret:%#x> <Line:%d>\n",ret,__LINE__);\
            return HI_FAILURE;\
        }\
        else\
        {\
            printf("Success! "str"\t<Line:%d>\n",__LINE__);\
        }\
    } while(0)
#define CHECKERR(str,ret)\
    do {\
        if (HI_SUCCESS != ret)\
        {\
            printf("ERROR! "str" <ret:%#x> <Line:%d>\n",ret,__LINE__);fflush(stdout);\
        }\
    } while(0)

volatile HI_BOOL g_bTimerQuit;
volatile HI_U32  fps = 0;
volatile HI_U32  bps = 0;
volatile HI_U32 old_fps = 0;
volatile HI_U32 old_bps = 0;
volatile int alrm_flag;
void timer(int sig)
{
    alrm_flag = 1;

    if (fps!=0 && bps!=0)
        printf(" fps:%2d, bps:%4d Kbps\n",fps-old_fps, 8*(bps-old_bps)/1024);
    old_fps = fps;
    old_bps = bps;
    printf("\n");
}
void *thread_timer(void *arg)
{
    int val = (int)arg;
    struct itimerval itv = {{0},{0}};

    signal(SIGALRM, timer);

    itv.it_interval.tv_sec = val;
    itv.it_value.tv_sec    = val;
    setitimer(ITIMER_REAL, &itv, NULL);

    while (!g_bTimerQuit)
    {
        pause();

        if (alrm_flag == 0)
            continue;

        alrm_flag = 0;
    }

    return NULL;
}

#define BBIT_TRACE(string...)\
            do\
            {\
                if (strcmp("bbit_vpp.c", __FILE__))\
                {\
                    printf("\n[File]:%s, [Func]:%s, [Line]:%d; ", __FILE__, __FUNCTION__, __LINE__);\
                    printf(string);\
                }\
            }while(0);


#define ADV7179_FILE    "/dev/misc/adv7179"
#define MT9D131_FILE    "/dev/misc/mt9d131"

int g_fdadv7179 = -1;
int g_fdMt9d131 = -1;

int adv7179_open()
{
    g_fdadv7179 = open(ADV7179_FILE,O_RDWR);
    if (g_fdadv7179 < 0)
    {
        BBIT_TRACE("can't open 7179\n");
        return -1;
    }
    BBIT_TRACE("open 7179 OK\n");
    return 0;
}

static HI_S32 adv7179_setnorm(VIDEO_NORM_E stComposeMode)
{
    //unsigned int tmp;

    if(stComposeMode==VIDEO_ENCODING_MODE_PAL)
    {
        if(0!=ioctl(g_fdadv7179,ENCODER_SET_NORM,VIDEO_MODE_656_PAL))
        {
            printf("7179 errno %d\r\n",errno);
            return HI_FAILURE;
        }
    }
    else if(stComposeMode==VIDEO_ENCODING_MODE_NTSC)
    {
        if(0!=ioctl(g_fdadv7179,ENCODER_SET_NORM,VIDEO_MODE_656_NTSC))
        {
            printf("7179 errno %d\r\n",errno);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

int adv7179_close()
{
    close(g_fdadv7179);
    return 0;
}

int mt9d131_open()
{
    g_fdMt9d131 = open(MT9D131_FILE,O_RDWR);
    if (g_fdMt9d131 < 0)
    {
        printf("can't open mt9d131\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
void mt9d131_close()
{
    if (g_fdMt9d131 > 0)
    {
        close(g_fdMt9d131);
    }
}
int mt9d131_set_uxga()
{
	int size=DC_VAL_XXGA;   
	if( 1600*1200 == IMAGE_SIZE)
	{
		size=DC_VAL_UXGA;
	}
	
	if (0 != ioctl(g_fdMt9d131,DC_SET_IMAGESIZE,&size))
	{
	    printf("mt9d131 errno %d\r\n",errno);
	    return HI_FAILURE;
	}

	return 0;
}

int sys_init(int flag)
{
    MPP_SYS_CONF_S stSysConf = {0};
    VB_CONF_S stVbConf ={0};

    HI_S32 ret;

    if (1600*1200 == flag)
    {
        stVbConf.u32MaxPoolCnt = 64;
        stVbConf.astCommPool[0].u32BlkSize = 1600*1200*2;
        stVbConf.astCommPool[0].u32BlkCnt = 10;
        stVbConf.astCommPool[1].u32BlkSize = 768*576*2;
        stVbConf.astCommPool[1].u32BlkCnt = 5;
    }
    else if (1280*720 == flag)
    {
        stVbConf.u32MaxPoolCnt = 64;
        stVbConf.astCommPool[0].u32BlkSize = 1344*720*2;
        stVbConf.astCommPool[0].u32BlkCnt = 6;
    }
    else
    {
        stVbConf.u32MaxPoolCnt = 64;
        stVbConf.astCommPool[0].u32BlkSize = 768*576*2;
        stVbConf.astCommPool[0].u32BlkCnt = 50;
        stVbConf.astCommPool[1].u32BlkSize = 384*288*2;
        stVbConf.astCommPool[1].u32BlkCnt = 50;
        stVbConf.astCommPool[2].u32BlkSize = 192*144*2;
        stVbConf.astCommPool[2].u32BlkCnt = 60;
    }

    ret = HI_MPI_VB_SetConf(&stVbConf);
    CHECKRET("HI_MPI_VB_SetConf", ret);

    ret = HI_MPI_VB_Init();
    CHECKRET("HI_MPI_VB_Init", ret);

    stSysConf.u32AlignWidth = 64;
    ret = HI_MPI_SYS_SetConf(&stSysConf);
    CHECKRET("HI_MPI_SYS_SetConf", ret);

    ret = HI_MPI_SYS_Init();
    CHECKRET("HI_MPI_SYS_Init", ret);

    return 0;
}
int sys_exit()
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();
    return 0;
}

int vi_open_digitalcamera(VI_DEV ViDevid,VI_CHN ViChn)
{
    VI_PUB_ATTR_S ViPubAttr;
    VI_CHN_ATTR_S ViChnAttr;
    HI_S32 ret;

    memset(&ViPubAttr,0,sizeof(VI_PUB_ATTR_S));
    memset(&ViChnAttr,0,sizeof(VI_CHN_ATTR_S));

    ViPubAttr.enInputMode = VI_MODE_DIGITAL_CAMERA;
    ViPubAttr.enWorkMode  = VI_WORK_MODE_2D1;
    ViPubAttr.u32AdType   = AD_2815;
    ViPubAttr.enViNorm    = VIDEO_ENCODING_MODE_PAL;
    ret = HI_MPI_VI_SetPubAttr(ViDevid,&ViPubAttr);
    CHECKRET("HI_MPI_VI_SetPubAttr",ret);

    ret = HI_MPI_VI_Enable(ViDevid);
    CHECKRET("HI_MPI_VI_Enable",ret);

    ViChnAttr.stCapRect.s32X = 0;
    ViChnAttr.stCapRect.s32Y = 0;
    ViChnAttr.stCapRect.u32Width  = IMAGE_WIDTH;//1600
    ViChnAttr.stCapRect.u32Height = IMAGE_HEIGHT;//1200
    ViChnAttr.enCapSel        = VI_CAPSEL_BOTH;
    ViChnAttr.bDownScale      = HI_FALSE;
    ViChnAttr.bChromaResample = HI_FALSE;
    ViChnAttr.enViPixFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    ret = HI_MPI_VI_SetChnAttr(ViDevid,ViChn,&ViChnAttr);
    CHECKRET("HI_MPI_VI_SetChnAttr",ret);

    ret = HI_MPI_VI_EnableChn(ViDevid,ViChn);
    CHECKRET("HI_MPI_VI_EnableChn",ret);

    ret = HI_MPI_VI_BindOutput(ViDevid,ViChn,0);
    CHECKRET("HI_MPI_VI_BindOutput",ret);

    return 0;
}
int vi_close(VI_DEV ViDevid,VI_CHN ViChn)
{
    HI_S32 ret;

    ret = HI_MPI_VI_DisableChn(ViDevid, ViChn);
    CHECKRET("HI_MPI_VI_DisableChn", ret);
    ret = HI_MPI_VI_Disable(ViDevid);
    CHECKRET("HI_MPI_VI_Disable", ret);

    return 0;
}

#define RED   0xff0000
#define GREEN 0x00ff00
#define BLUE  0x0000ff
int vo_open(VO_CHN VoChn, VIDEO_NORM_E enComposeMode)
{
    VO_PUB_ATTR_S VoPubAttr;
    VO_CHN_ATTR_S VoChnAttr;
    HI_U32 u32VoFramerate;
    HI_S32 ret;

    memset(&VoPubAttr,0,sizeof(VO_PUB_ATTR_S));
    memset(&VoChnAttr,0,sizeof(VO_CHN_ATTR_S));

    //adv7179_open();
    //adv7179_setnorm(enComposeMode);

    VoPubAttr.stTvConfig.stComposeMode = enComposeMode;
    VoPubAttr.u32BgColor = BLUE;
    ret = HI_MPI_VO_SetPubAttr(&VoPubAttr);
    CHECKRET("HI_MPI_VO_SetPubAttr",ret);

    ret = HI_MPI_VO_Enable();
    CHECKRET("HI_MPI_VO_Enable",ret);

    VoChnAttr.u32Priority = 1;
    VoChnAttr.stRect.s32X = 0;
    VoChnAttr.stRect.s32Y = 0;
    VoChnAttr.stRect.u32Width  = 720;
    if (VIDEO_ENCODING_MODE_PAL == enComposeMode)
    {
        VoChnAttr.stRect.u32Height = 576;
        u32VoFramerate = 25;
    }
    else if (VIDEO_ENCODING_MODE_NTSC == enComposeMode)
    {
        VoChnAttr.stRect.u32Height = 480;
        u32VoFramerate = 30;
    }
    VoChnAttr.bZoomEnable = HI_TRUE;
    ret = HI_MPI_VO_SetChnAttr(VoChn,&VoChnAttr);
    CHECKRET("HI_MPI_VO_SetChnAttr",ret);

    ret = HI_MPI_VO_EnableChn(VoChn);
    CHECKRET("HI_MPI_VO_EnableChn",ret);

    ret = HI_MPI_VO_SetFrameRate(VoChn,u32VoFramerate);
    CHECKRET("HI_MPI_VO_SetFrameRate",ret);

    return 0;
}
int vo_close(VO_CHN VoChn)
{
    HI_S32 ret;

    ret = HI_MPI_VO_DisableChn(VoChn);
    CHECKRET("HI_MPI_VO_DisableChn", ret);
    ret = HI_MPI_VO_Disable();
    CHECKRET("HI_MPI_VO_Disable", ret);

    return 0;
}

int venc_open_digitalcamera(VENC_GRP VencGroup, VENC_CHN VencChn)
{
    VENC_CHN_ATTR_S  stVencAttr;
    VENC_ATTR_H264_S stH264Attr;
    HI_S32 ret;

    stH264Attr.u32Priority         = 0;
    stH264Attr.u32PicWidth         = IMAGE_WIDTH;//1600
    stH264Attr.u32PicHeight        = IMAGE_HEIGHT;//1200
    stH264Attr.u32ViFramerate      = 30;
    stH264Attr.u32TargetFramerate  = 30;
    stH264Attr.bMainStream         = 1;
    //stH264Attr.bVIField            = 1;
    stH264Attr.bField              = 0;
    stH264Attr.bCBR                = 1;
    stH264Attr.u32Bitrate          = 4096;
    stH264Attr.u32PicLevel         = 0;
    stH264Attr.u32Gop              = 50;
    stH264Attr.u32MaxDelay         = 10;
    stH264Attr.u32BufSize          = IMAGE_WIDTH*IMAGE_HEIGHT*2;
    stH264Attr.bByFrame            = 1;
    stVencAttr.enType = PT_H264;
    stVencAttr.pValue = (void *)&stH264Attr;
    ret = HI_MPI_VENC_CreateGroup(VencGroup);
    CHECKRET("HI_MPI_VENC_CreateGroup",ret);

    ret = HI_MPI_VENC_CreateChn(VencChn, &stVencAttr, NULL);
    CHECKRET("HI_MPI_VENC_CreateChn",ret);

    ret = HI_MPI_VENC_RegisterChn(VencGroup, VencChn);
    CHECKRET("HI_MPI_VENC_RegisterChn",ret);

    return 0;
}
int venc_close(VENC_GRP VencGroup, VENC_CHN VencChn)
{
    HI_S32 ret;

    ret = HI_MPI_VENC_UnRegisterChn(VencChn);
    CHECKRET("HI_MPI_VENC_UnRegisterChn",ret);
    ret = HI_MPI_VENC_DestroyChn(VencChn);
    CHECKRET("HI_MPI_VENC_DestroyChn",ret);
    ret = HI_MPI_VENC_DestroyGroup(VencGroup);
    CHECKRET("HI_MPI_VENC_DestroyGroup",ret);

    return 0;
}

HI_U8 g_filename[128];
#define FILEPATH "./stream/"
#define SAVEFILE
volatile HI_BOOL g_bVencQuit;
/* 获取并处理视频编码流 */
int venc_StreamProcess(void *arg)
{
    VENC_CHN VencChn = (VENC_CHN)arg;
    VENC_STREAM_S stVencStream;
    VDEC_STREAM_S stVdecStream;
    VENC_CHN_STAT_S stStat;
    HI_U32 streamLen = 0;
    HI_U32 u32FrameNum = 0;
    HI_S32 ret;
    HI_S32 i;

    FILE *pf = NULL;
    fd_set ReadFds;
    HI_S32 s32VencFd;
    struct timeval TimeoutVal;
    struct timeval t1,t2;

    pf = fopen(g_filename,"wb");
    if (!pf)
    {
        printf("fopen '%s' failed!\n",g_filename);
        return -1;
    }
    else
    {
        printf("Create file '%s' OK.\n",g_filename);
    }

    s32VencFd = HI_MPI_VENC_GetFd(VencChn);
    gettimeofday(&t1,NULL);
    while (!g_bVencQuit) /*Get Stream and Save to File opened above*/
    {
#define TIMEINTERVAL 60*10 /* 30分钟存一个文件 */
        gettimeofday(&t2,NULL);
        //printf("time interval = %d\n",t2.tv_sec - t1.tv_sec);

        FD_ZERO(&ReadFds);
        FD_SET(s32VencFd,&ReadFds);

        TimeoutVal.tv_sec = 5;
        TimeoutVal.tv_usec = 0;

        ret = select(s32VencFd+1, &ReadFds, NULL, NULL, &TimeoutVal);
        if (ret < 0)
        {
            perror("select");
            if (g_bVencQuit)
                return -1;
        }
        else if (0 == ret) /* time out */
        {
            printf("Time out!\n");
            if (g_bVencQuit)
                return 0;
        }
        else
        {
            if (FD_ISSET(s32VencFd, &ReadFds))
            {
                ret = HI_MPI_VENC_Query(VencChn, &stStat);
                CHECKERR("HI_MPI_VENC_Query", ret);

                stVencStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                stVencStream.u32PackCount = stStat.u32CurPacks;
                if (!stVencStream.pstPack)
                {
                    free(stVencStream.pstPack);
                    return -1;
                }

                do
                {
                    ret = HI_MPI_VENC_GetStream(VencChn,&stVencStream,HI_IO_NOBLOCK);
                    CHECKERR("HI_MPI_VENC_GetStream", ret);
                    if (g_bVencQuit)
                        break;
                } while (ret != HI_SUCCESS);
                //printf("stVencStream.u32PackCount = %d\n",stVencStream.u32PackCount);
                //for (i=0; i<stVencStream.u32PackCount; i++)
                    //printf("stVencStream.pstPack[%d].DataType.enH264EType = %d\n",i,stVencStream.pstPack[i].DataType.enH264EType);

                u32FrameNum++;
                fps = u32FrameNum;

                for (i=0; i<stVencStream.u32PackCount; i++)
                {
                    streamLen += stVencStream.pstPack[i].u32Len[0] + stVencStream.pstPack[i].u32Len[1];
                }
                bps = streamLen;

                for (i=0; i<stVencStream.u32PackCount; i++)
                {
                    stVdecStream.pu8Addr = stVencStream.pstPack[i].pu8Addr[0];
                    stVdecStream.u32Len  = stVencStream.pstPack[i].u32Len[0];
                    stVdecStream.u64PTS  = stVencStream.pstPack[i].u64PTS;
                    fwrite(stVencStream.pstPack[i].pu8Addr[0], stVencStream.pstPack[i].u32Len[0], 1, pf);
                    if (stVencStream.pstPack[i].u32Len[1] > 0)
                    {
                        stVdecStream.pu8Addr = stVencStream.pstPack[i].pu8Addr[1];
                        stVdecStream.u32Len  = stVencStream.pstPack[i].u32Len[1];
                        stVdecStream.u64PTS  = stVencStream.pstPack[i].u64PTS;
                        fwrite(stVencStream.pstPack[i].pu8Addr[1], stVencStream.pstPack[i].u32Len[1], 1, pf);
                    }
                    fflush(pf);
                }

                ret = HI_MPI_VENC_ReleaseStream(VencChn, &stVencStream);
                CHECKERR("HI_MPI_VENC_ReleaseStream",ret);

                free(stVencStream.pstPack);
                stVencStream.pstPack = NULL;
            }
        }
    }

    if (pf)
    {
        fclose(pf);
        pf = NULL;
    }

    return 0;
}

pthread_t thrd_timer,thrd_venc,thrd_vdec,thrd_av,thrd_md;
int venc_start(VI_DEV ViDevid,VI_CHN ViChn,VENC_GRP VencGroup,VENC_CHN VencChn)
{
    HI_S32 ret;

    ret = HI_MPI_VENC_BindInput(VencGroup, ViDevid, ViChn);
    CHECKRET("HI_MPI_VENC_BindInput",ret);

    ret = HI_MPI_VENC_StartRecvPic(VencChn);
    CHECKRET("HI_MPI_VENC_StartRecvPic",ret);

    printf("input file path/name (eg./mnt/1.h264):");
    scanf("%s",g_filename);getchar();
    printf("filename:'%s'\n",g_filename);

    g_bTimerQuit = HI_FALSE;
    if ( pthread_create(&thrd_timer, NULL, thread_timer, (void*)1) )
        printf("Timer start failed!\n");
    else
        printf("Timer start success!\n");/**/

    g_bVencQuit = HI_FALSE;
    if ( pthread_create(&thrd_venc, NULL, (void*)venc_StreamProcess, (void*)VencChn) )
    {
        printf("Encoding thread create failed!\n");
        return HI_FAILURE;
    }

    return 0;
}
int venc_stop(VENC_GRP VencGroup,VENC_CHN VencChn)
{
    HI_S32 ret;

    g_bVencQuit = HI_TRUE;
    g_bTimerQuit = HI_TRUE;
    pthread_join(thrd_venc,NULL);
    pthread_join(thrd_timer,NULL);

    ret = HI_MPI_VENC_StopRecvPic(VencChn);
    CHECKRET("HI_MPI_VENC_StopRecvPic",ret);

    ret = HI_MPI_VENC_UnbindInput(VencGroup);
    CHECKRET("HI_MPI_VENC_UnbindInput",ret);

    return 0;
}

int main(int argc, char *argv[])
{
    VI_DEV ViDevid = 0;
    VI_CHN ViChn = 0;
    VENC_GRP VencGroup = 0;
    VENC_CHN VencChn = 0;
    VO_CHN VoChn = 0;
    VIDEO_NORM_E enComposeMode = VIDEO_ENCODING_MODE_PAL;

	int pixelnum;
    pixelnum = atoi(argv[1]);

    if(200 == pixelnum)
    	{
 		IMAGE_SIZE=1600*1200;
		IMAGE_WIDTH=1600;
		IMAGE_HEIGHT=1200;
    	}
    else if(100 == pixelnum)
    	{
		IMAGE_SIZE=1280*720;
		IMAGE_WIDTH=1280;
		IMAGE_HEIGHT=720;
   	}
    else
    	{
	    	printf("usage: sample_hd 200/100");
	    	return -1;
    	}

    sys_exit();
    sys_init(IMAGE_HEIGHT*IMAGE_WIDTH);

    mt9d131_open();
    mt9d131_set_uxga();

    vo_open(VoChn, enComposeMode);
    vi_open_digitalcamera(ViDevid, ViChn);
    venc_open_digitalcamera(VencGroup, VencChn);
    venc_start(ViDevid, ViChn, VencGroup, VencChn);

    printf("Press ENTER to Quit!\n");
    getchar();

    venc_stop(VencGroup, VencChn);
    venc_close(VencGroup, VencChn);
    vi_close(ViDevid, ViChn);
    vo_close(VoChn);

    mt9d131_close();

    return 0;
}

