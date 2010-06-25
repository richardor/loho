#include "vio.h"



#define G_VIDEV_START 0


typedef struct _VideoInitPara_{
    tvsystem_t system;
    resolution_t resolution;
    int channel_maxnum;
}VideoInitPara;





static bool m_videoinitflag = false;
VideoInitPara m_VideoInitPara;
VO_DEV_E m_VideoOutDev = VO_DEV_SD;



int DisableVideoOutput(VO_DEV VoDev)
{
	DEBUG_INFO(("DisableVideoOutput starttttt\n"));
	
    int ibind = 16;
    int i = 0;
	int iRet = -1;

    
	for(i = 0; i < ibind; i ++)//最大通道数
	{
		if(iRet = (HI_MPI_VO_DisableChn(VoDev,i)) != 0)
		{
			DEBUG_INFO(("DisableVideoOutput HI_MPI_VO_DisableChn err (0x%x)  vochn=%d \n",iRet, i));
			//return -1;
		}
	}
	return 0;

}
static int CheckVideoOutputMode(outputmode_t outputmode)
{
    if((outputmode == OM_ONE) || \
        (outputmode == OM_FOUR) || \
        (outputmode == OM_NINE) ||\
        (outputmode == OM_SIXTEEN))
    {
        return 0;
    }
    return -1;
}

static void CleanStartLogo()
{
	HI_MPI_SYS_SetReg(0x20130600,0x40000049);
	HI_MPI_SYS_SetReg(0x20130604,0x00000001);
	HI_MPI_SYS_SetReg(0x20130700,0x40000049);
	HI_MPI_SYS_SetReg(0x20130704,0x00000001);
}


static int VichnPerDev(VI_INPUT_MODE_E enViInputMode, VI_WORK_MODE_E enViWorkMode)
{
    if ((VI_MODE_BT656 == enViInputMode) && (VI_WORK_MODE_1D1 == enViWorkMode))
    {
        return 1;
    }
    else if ((VI_MODE_BT656 == enViInputMode) && (VI_WORK_MODE_2D1 == enViWorkMode))
    {
        return 2;
    }
    else if ((VI_MODE_BT656 == enViInputMode) && (VI_WORK_MODE_4D1 == enViWorkMode))
    {
        return 4;
    }
    else if ((VI_MODE_BT656 == enViInputMode) && (VI_WORK_MODE_4HALFD1 == enViWorkMode))
    {
        return 4;
    }
    else if ((VI_MODE_BT601 == enViInputMode) || (VI_MODE_DIGITAL_CAMERA == enViInputMode)
             || (VI_MODE_BT1120_PROGRESSIVE == enViInputMode) || (VI_MODE_BT1120_INTERLACED == enViInputMode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static HI_S32 SAMPLE_GetViChnPerDev(VI_DEV ViDev)
{
    VI_PUB_ATTR_S stPubAttr;
    HI_S32 s32ViChnPerDev;
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_GetPubAttr(ViDev, &stPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Get vi(%d) channel per dev fail\n", ViDev);
        return 0;
    }

    s32ViChnPerDev = VichnPerDev(stPubAttr.enInputMode, stPubAttr.enWorkMode);
    return s32ViChnPerDev;
}
static HI_S32 ViBindVo(HI_S32 s32ViChnTotal, VO_DEV VoDev)
{
    HI_S32 ViChnCnt;
    HI_S32 s32ViChnPerDev;
    VI_DEV ViDev;
    VO_CHN VoChn;
    VI_CHN ViChn;
    HI_S32 s32Ret;

    s32ViChnPerDev = SAMPLE_GetViChnPerDev(0);

    /* Bind VI channel to VO channel */
    ViDev = G_VIDEV_START;
    VoChn = 0;
    ViChnCnt = 0;
    while (1)
    {
        for (ViChn = 0; ViChn < s32ViChnPerDev; ViChn++, VoChn++)
        {
            s32Ret = HI_MPI_VI_BindOutput(ViDev, ViChn, VoDev, VoChn);
            if (0 != s32Ret)
            {
                printf("bind vi2vo failed, vi(%d,%d),vo(%d,%d)!\n",
                       ViDev, ViChn, VoDev, VoChn);
                return s32Ret;
            }

            ViChnCnt++;
            if (ViChnCnt >= s32ViChnTotal)
            {
                return HI_SUCCESS;
            }
        }
#ifdef hi3515
        ViDev += 2;
#else
        ViDev ++;
#endif
    }

    return HI_SUCCESS;
}

HI_S32 SetVoChnMScreen(VO_DEV VoDev, HI_U32 u32ChnCnt, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_U32 i, div, w, h, ret;
    VO_CHN_ATTR_S stChnAttr;

    /* If display 32 vo channels, should use 36-screen split. */
    u32ChnCnt = (u32ChnCnt == VO_MAX_CHN_NUM) ? 36 : u32ChnCnt;

    div = (int)sqrt(u32ChnCnt);
    w = (u32Width / div);
    h = (u32Height / div);

    for (i = 0; i < u32ChnCnt; i++)
    {
        if (i >= VO_MAX_CHN_NUM)
        {
            break;
        }

        stChnAttr.u32Priority = 0;
        stChnAttr.bZoomEnable = HI_TRUE;
        stChnAttr.bDeflicker  = (((1==u32ChnCnt) || (VoDev == 0)) ? HI_FALSE : HI_TRUE);
        stChnAttr.stRect.s32X = w * (i % div);
        stChnAttr.stRect.s32Y = h * (i / div);
        stChnAttr.stRect.u32Width  = w;
        stChnAttr.stRect.u32Height = h;

        if (stChnAttr.stRect.s32X % 2 != 0)
        {
            stChnAttr.stRect.s32X++;
        }

        if (stChnAttr.stRect.s32Y % 2 != 0)
        {
            stChnAttr.stRect.s32Y++;
        }

        if (stChnAttr.stRect.u32Width % 2 != 0)
        {
            stChnAttr.stRect.u32Width++;
        }

        if (stChnAttr.stRect.u32Height % 2 != 0)
        {
            stChnAttr.stRect.u32Height++;
        }

        ret = HI_MPI_VO_SetChnAttr(VoDev, i, &stChnAttr);
        if (ret != HI_SUCCESS)
        {
            printf("In %s set channel %d attr failed with %#x!\n", __FUNCTION__, i, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_StartVoVideoLayer(VO_DEV VoDev, VO_VIDEO_LAYER_ATTR_S* pstVideoLayerAttr)
{
    HI_S32 ret;

    /* set public attr of VO*/
    ret = HI_MPI_VO_SetVideoLayerAttr(VoDev, pstVideoLayerAttr);
    if (HI_SUCCESS != ret)
    {
        printf("set video layer of dev %u failed %#x!\n", VoDev, ret);
        return HI_FAILURE;
    }

    /* enable VO device*/
    ret = HI_MPI_VO_EnableVideoLayer(VoDev);
    if (HI_SUCCESS != ret)
    {
        printf("enable video layer of dev %d failed with %#x !\n", VoDev, ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_StartVoDevice(VO_DEV VoDev, VO_PUB_ATTR_S* pstDevAttr)
{
    HI_S32 ret;

	/*because we will change vo device attribution,
	  so we diable vo device first*/
	ret = HI_MPI_VO_Disable(VoDev);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_VO_Disable fail 0x%08x.\n", ret);
        return HI_FAILURE;
    }

    ret = HI_MPI_VO_SetPubAttr(VoDev, pstDevAttr);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_VO_SetPubAttr fail 0x%08x.\n", ret);
        return HI_FAILURE;
    }

    ret = HI_MPI_VO_Enable(VoDev);
    if (HI_SUCCESS != ret)
    {
        printf("HI_MPI_VO_Enable fail 0x%08x.\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 StartVo(HI_S32                 s32ChnTotal,
                        VO_DEV                 VoDev,
                        VO_PUB_ATTR_S*         pstVoDevAttr,
                        VO_VIDEO_LAYER_ATTR_S* pstVideoLayerAttr)
{
    HI_S32 s32Ret;
    HI_U32 u32Width;
    HI_U32 u32Height;
    VO_CHN VoChn;

    s32Ret = SAMPLE_StartVoDevice(VoDev, pstVoDevAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* set vo video layer attribute and start video layer. */
    s32Ret = SAMPLE_StartVoVideoLayer(VoDev, pstVideoLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* set vo channel attribute and start each channel. */
    u32Width  = pstVideoLayerAttr->stImageSize.u32Width;
    u32Height = pstVideoLayerAttr->stImageSize.u32Height;

    if (s32ChnTotal == 1)
    {
        ;
    }
    else if (s32ChnTotal <= 4)
    {
        s32ChnTotal = 4;
    }
    else if (s32ChnTotal <= 9)
    {
        s32ChnTotal = 9;
    }
    else if (s32ChnTotal <= 16)
    {
        s32ChnTotal = 16;
    }
    else if (s32ChnTotal <= 36)
    {
        /* 36-screen split is support. But only 32 vo channels display. */
        s32ChnTotal = VO_MAX_CHN_NUM;
    }
    else
    {
        printf("too many vo channels!\n");
        return HI_FAILURE;
    }

    s32Ret = SetVoChnMScreen(VoDev, s32ChnTotal, u32Width, u32Height);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    
    for (VoChn = 0; VoChn < s32ChnTotal; VoChn++)
    {
        s32Ret = HI_MPI_VO_EnableChn(VoDev, VoChn);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_VO_EnableChn(%d, %d) failed, err code:0x%08x\n\n",
                VoDev, VoChn, s32Ret);
            return HI_FAILURE;
        }
		s32Ret= HI_MPI_VO_ChnShow(VoDev, VoChn);
		if (0 !=s32Ret)
		{
			DEBUG_INFO(("err : 0x%08x,	line:%d\n",s32Ret,__LINE__));
			return -1;
		}
    }
    

    return HI_SUCCESS;
}


static void GetVodevAttrAndVoLayerAttr(VO_PUB_ATTR_S* pstVoDevAttr,VO_VIDEO_LAYER_ATTR_S* pstVideoLayerAttr)
{
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32DisplayRate;

    pstVoDevAttr->u32BgColor = 0xffffff;

    pstVoDevAttr->enIntfType = VO_INTF_CVBS;

    pstVoDevAttr->enIntfSync = VO_OUTPUT_PAL;

    u32Width  = 720;
    u32Height = 576;
    u32DisplayRate = 25;
    
    pstVideoLayerAttr->stDispRect.s32X = 0;
    pstVideoLayerAttr->stDispRect.s32Y = 0;
    pstVideoLayerAttr->stDispRect.u32Width   = u32Width;
    pstVideoLayerAttr->stDispRect.u32Height  = u32Height;
    pstVideoLayerAttr->stImageSize.u32Width  = u32Width;
    pstVideoLayerAttr->stImageSize.u32Height = u32Height;
    pstVideoLayerAttr->u32DispFrmRt = u32DisplayRate;
    pstVideoLayerAttr->enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    pstVideoLayerAttr->s32PiPChn = VO_DEFAULT_CHN;

    return;
}




static int StartViByChn(HI_S32 s32ChnTotal, VI_PUB_ATTR_S* pstViDevAttr, VI_CHN_ATTR_S* pstViChnAttr, tvsystem_t system)
{
    VI_DEV ViDev,vi_start;
    HI_S32 s32ChnPerDev;
    HI_S32 s32DevTotal;
    HI_S32 ViChn;
    HI_S32 s32Ret;
    HI_S32 s32ChnCnt;
    HI_U32 u32SrcFrmRate;

    s32ChnPerDev = VichnPerDev(pstViDevAttr->enInputMode, pstViDevAttr->enWorkMode);
    s32DevTotal = (s32ChnTotal - 1) / s32ChnPerDev + 1;
    u32SrcFrmRate = (system== TS_PAL) ? 25 : 30;

    s32ChnCnt = 0;
    vi_start = 0;
    for (ViDev = vi_start; ViDev < vi_start + s32DevTotal; ViDev++)
    {
        s32Ret = HI_MPI_VI_SetPubAttr(ViDev, pstViDevAttr);
        if (s32Ret!=0)
        {
            DEBUG_INFO(("HI_MPI_VI_SetPubAttr %d attr fail, HI_MPI_VI_SetPubAttr 0x%x\n", ViDev, s32Ret));
            return HI_FAILURE;
        }

        s32Ret = HI_MPI_VI_Enable(ViDev);
        if (s32Ret!=0)
        {
            DEBUG_INFO(("VI dev %d enable fail ,HI_MPI_VI_Enable err(0x%x)\n", ViDev, s32Ret));
            return s32Ret;
        }

        printf("enable vi dev %d ok\n", ViDev);

        for (ViChn = 0; ViChn < s32ChnPerDev; ViChn++)
        {
            s32Ret = HI_MPI_VI_SetChnAttr(ViDev, ViChn, pstViChnAttr);
            if (s32Ret!=0)
            {
                DEBUG_INFO(("set vi(%d,%d) attr fail, HI_MPI_VI_SetChnAttr err%x\n", ViDev, ViChn, s32Ret));
                return s32Ret;
            }

            s32Ret = HI_MPI_VI_EnableChn(ViDev, ViChn);
            if (s32Ret!=0)
            {
                DEBUG_INFO(("set vi(%d,%d) attr fail,HI_MPI_VI_EnableChn err %x\n", ViDev, ViChn, s32Ret));
                return s32Ret;
            }

            s32Ret = HI_MPI_VI_SetSrcFrameRate(ViDev, ViChn, u32SrcFrmRate);
            if (s32Ret!=0)
            {
                DEBUG_INFO(("set vi(%d,%d) attr fail,HI_MPI_VI_SetSrcFrameRate err%x\n", ViDev, ViChn, s32Ret));
                return s32Ret;
            }

            printf("enable vi(%d,%d) ok\n", ViDev, ViChn);

            /* return when all channel ok. */
            if(++s32ChnCnt >= s32ChnTotal)
            {
                return HI_SUCCESS;
            }
        }
    }
    return HI_SUCCESS;
}

static int GetVidevAndChannelAttr(VI_PUB_ATTR_S* pstViDevAttr,VI_CHN_ATTR_S* pstViChnAttr)
{   
    DEBUG_INFO(("GetVidevAndChannelAttr: 11111111111111111111111\n"));
    pstViDevAttr->enInputMode = VI_MODE_BT656;
    pstViDevAttr->enWorkMode = VI_WORK_MODE_4D1;
    if(m_VideoInitPara.system == TS_PAL)
    {
        DEBUG_INFO(("HI_MPI_VI_SetChnAttr TS_PAL1111111111111\n"));
        pstViDevAttr->enViNorm = VIDEO_ENCODING_MODE_PAL;
    }
    else
    {
        pstViDevAttr->enViNorm = VIDEO_ENCODING_MODE_NTSC;
    }
    pstViChnAttr->stCapRect.u32Width  = 704;
    if(m_VideoInitPara.system == TS_PAL)
    {
        DEBUG_INFO(("HI_MPI_VI_SetChnAttr TS_PAL22222222222222222\n"));
        pstViChnAttr->stCapRect.u32Height = 288;
    }
    else
    {
        pstViChnAttr->stCapRect.u32Height = 240;
    }
    pstViChnAttr->stCapRect.s32X = 8;
    pstViChnAttr->stCapRect.s32Y = 0;
    pstViChnAttr->enViPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    pstViChnAttr->bHighPri = HI_FALSE;
    pstViChnAttr->bChromaResample = HI_FALSE;

    if(m_VideoInitPara.resolution == RES_CIF)
    {
        DEBUG_INFO(("m_VideoInitPara.resolution == RES_CIF\n"));
        pstViChnAttr->bDownScale = HI_TRUE;//CIF
        pstViChnAttr->enCapSel = VI_CAPSEL_BOTH;
    }
    else if(m_VideoInitPara.resolution == RES_HD1)
    {
        pstViChnAttr->bDownScale = HI_FALSE;
        pstViChnAttr->enCapSel = VI_CAPSEL_BOTH;//HD1
    }
    else if(m_VideoInitPara.resolution == RES_D1)
    {
        pstViChnAttr->bDownScale = HI_FALSE;
        pstViChnAttr->enCapSel = VI_CAPSEL_BOTH;//D1
    }
    else
    {
        pstViChnAttr->bDownScale = HI_FALSE;
        pstViChnAttr->enCapSel = VI_CAPSEL_BOTH;
    }
    return 0;
    return -1;

}
    

void InitPlatform()
{
    DEBUG_INFO(("InitPlatform\n"));
    MPP_SYS_CONF_S stSysConf = {0};
    VB_CONF_S stVbConf ={0};
    HI_S32 ret=0;

	KEY_INFO(("InitPlatform HI_MPI_SYS_Exit\n"));
    ret=HI_MPI_SYS_Exit(); 
    if(ret != 0)
    {
        KEY_INFO(("InitPlatform HI_MPI_SYS_Exit failed    err(0x%x) LINE(%d)\n", ret, __LINE__));
    }

	KEY_INFO(("InitPlatform HI_MPI_VB_Exit\n"));
    ret=HI_MPI_VB_Exit();
    if(ret!= 0)
    {
        KEY_INFO(("InitPlatform HI_MPI_VB_Exit failed   err(0x%x), LINE(%d)\n", ret, __LINE__));
        //exit(1);
    }
    
	stVbConf.astCommPool[0].u32BlkSize = 720 * 576 * 2;/*D1*/
    stVbConf.astCommPool[0].u32BlkCnt  = 10;
    stVbConf.astCommPool[1].u32BlkSize = 704 * 576 * 2;/*D1*/
    stVbConf.astCommPool[1].u32BlkCnt  = 52;
    stVbConf.astCommPool[2].u32BlkSize = 384 * 576 * 2;/*2CIF*/
    stVbConf.astCommPool[2].u32BlkCnt = 64;
    stVbConf.astCommPool[3].u32BlkSize = 384 * 288 * 2;/*CIF*/
    stVbConf.astCommPool[3].u32BlkCnt  = 20;

	KEY_INFO(("InitPlatform HI_MPI_VB_SetConf\n"));
    if (HI_MPI_VB_SetConf(&stVbConf))
    {
        KEY_INFO(("HI_MPI_VB_SetConf failed!\n"));
        exit(1);
    }

    if (HI_MPI_VB_Init())
    {
        printf("HI_MPI_VB_Init failed!\n");
        exit(1);
    }

	KEY_INFO(("InitPlatform HI_MPI_SYS_SetConf\n"));
    stSysConf.u32AlignWidth = 64;
    if (HI_MPI_SYS_SetConf(&stSysConf))
    {
        printf("conf : system config failed!\n");
        exit(1);
    }

	KEY_INFO(("InitPlatform HI_MPI_SYS_Init\n"));
    if (HI_MPI_SYS_Init())
    {
        printf("sys init failed!\n");
        exit(1);
    }
    
#if 1//AoDevId == 1
	unsigned int regval;
	HI_MPI_SYS_GetReg(0x20050054,&regval);
	HI_MPI_SYS_SetReg(0x20050054,regval|0x2000000);
	HI_MPI_SYS_SetReg(0x200f0110,0x00000002);
	HI_MPI_SYS_SetReg(0x200f0114,0x00000002);
	HI_MPI_SYS_SetReg(0x200f0118,0x00000002);
#endif    
    
    return;
}


void VideoInit(tvsystem_t system, resolution_t resolution, int max_channel_num)
{   
    
    if(!m_videoinitflag)
    {
    
        DEBUG_INFO(("VideoInit system(%d)[0 pal 1 ntsc] resolution(%d) max_channel_num(%d)\n",system, resolution,max_channel_num));
        
        m_VideoInitPara.system = system;
        m_VideoInitPara.resolution = resolution;
        m_VideoInitPara.channel_maxnum = max_channel_num;

        m_videoinitflag = true;
    }
    
        
	return;
}

void VideoUninit()
{
    if(m_videoinitflag)
    {
        DEBUG_INFO(("VideoUninit\n"));
        m_videoinitflag = false;
    }
    return ;
}

int StartVideoOut()
{ 
    if(false == m_videoinitflag)
    {
        DEBUG_INFO(("StartVideoOut failed : [m_videoinitflag == false]"));
        return -1;
    }
    
    VI_PUB_ATTR_S pstViDevAttr;
    VI_CHN_ATTR_S pstViChnAttr;
    
    VO_PUB_ATTR_S stVoDevAttr;
    VO_VIDEO_LAYER_ATTR_S stVideoLayerAttr;

    DEBUG_INFO(("StartVideoOut\n"));

    DEBUG_INFO(("GetVidevAndChannelAttr\n"));
    GetVidevAndChannelAttr(&pstViDevAttr, &pstViChnAttr);

    DEBUG_INFO(("StartViByChn\n"));
    StartViByChn(m_VideoInitPara.channel_maxnum, &pstViDevAttr, &pstViChnAttr, m_VideoInitPara.system);

    DEBUG_INFO(("GetVodevAttrAndVoLayerAttr\n"));
    GetVodevAttrAndVoLayerAttr(&stVoDevAttr, &stVideoLayerAttr);

    DEBUG_INFO(("StartVo\n"));
    StartVo(m_VideoInitPara.channel_maxnum, m_VideoOutDev, &stVoDevAttr, &stVideoLayerAttr);

    DEBUG_INFO(("ViBindVo\n"));
    ViBindVo(m_VideoInitPara.channel_maxnum, m_VideoOutDev);
    
    CleanStartLogo();

    DEBUG_INFO(("StartVideoOut success!"));

	return 0;
}

int ChangeVideoOut(outputmode_t outputmode, channelid_t channel[MAX_VIDEO_NUM])
{
    DEBUG_INFO(("ChangeVideoOut outputmode(%d)[0:one 1:four 2:nine 3:sixteen] channel[0](%d)\n", outputmode, channel[0]));
    
    if(CheckVideoOutputMode(outputmode) != 0)
    {
         DEBUG_INFO(("ChangeVideoOut : check outputmode failed!\n")); 
         return -1;
    }

    int ScreenDiv = 16;
    HI_S32 s32Ret;
    HI_S32 div;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    HI_S32 u32PicWidth;
    HI_S32 u32PicHeight;
    int i = 0;
    int VoChn;
    VO_CHN_ATTR_S VoChnAttr;

    
    /* 1 get ScreenDiv num*/
    switch(outputmode)
    {
        case OM_ONE:
            ScreenDiv = 1;
            break;
        case OM_FOUR:
            ScreenDiv = 4;
            break;
        case OM_NINE:
            ScreenDiv = 9;
            break;
        case OM_SIXTEEN:
        default:
            ScreenDiv = 16;
            break;
    }

    /*2 设置属性开始。*/    
#if 0
    for (i = 0; i < VO_MAX_CHN_NUM; i++)
    {
        HI_MPI_VO_ChnHide(m_VideoOutDev, i);
    }
#endif
    DisableVideoOutput(m_VideoOutDev);

    div = (int)sqrt(ScreenDiv);
    DEBUG_INFO(("div(%d)\n", div));
    

    s32Ret = HI_MPI_VO_GetVideoLayerAttr(m_VideoOutDev, &stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_GetVideoLayerAttr(%d) errcode: 0x%08x\n",
            m_VideoOutDev, s32Ret);
        return HI_FAILURE;
    }
    u32PicWidth  = stLayerAttr.stDispRect.u32Width / div;
    u32PicHeight = stLayerAttr.stDispRect.u32Height / div;
    DEBUG_INFO(("u32PicWidth(%d) u32PicHeight(%d)",u32PicWidth, u32PicHeight));

    for (i = 0; i < ScreenDiv; i++)
    {
        VoChn = i;
        VoChnAttr.stRect.s32X = (i % div) * u32PicWidth;
        VoChnAttr.stRect.s32Y = (i / div) * u32PicHeight;
        VoChnAttr.stRect.u32Width  = u32PicWidth;
        VoChnAttr.stRect.u32Height = u32PicHeight;
        DEBUG_INFO(("i(%d), s32X(%d), VoChnAttr.stRect.s32Y(%d)\n", i, VoChnAttr.stRect.s32X, VoChnAttr.stRect.s32Y));
        VoChnAttr.u32Priority = 1;
        VoChnAttr.bZoomEnable = HI_TRUE;
        VoChnAttr.bDeflicker  = (1==ScreenDiv) ? HI_FALSE : HI_TRUE;
        if (0 != HI_MPI_VO_SetChnAttr(m_VideoOutDev, VoChn, &VoChnAttr))
        {
            printf("set VO Chn %d attribute(%d,%d,%d,%d) failed !\n",
                   VoChn, VoChnAttr.stRect.s32X, VoChnAttr.stRect.s32Y,
                   VoChnAttr.stRect.u32Width, VoChnAttr.stRect.u32Height);
            return -1;
        }

        s32Ret = HI_MPI_VO_EnableChn(m_VideoOutDev, VoChn);
        if (HI_SUCCESS != s32Ret)
        {
            DEBUG_INFO(("HI_MPI_VO_EnableChn(%d, %d) failed, err code:0x%08x   LIINE(%d)\n\n", m_VideoOutDev, i, s32Ret, __LINE__));
            return HI_FAILURE;
        }

    }


#if 1
    if (0 != HI_MPI_VO_SetAttrEnd(m_VideoOutDev))
    {
        return -1;
    }
#endif
 

    
    return 0;
}



