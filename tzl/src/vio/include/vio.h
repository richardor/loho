#ifndef __VIO_H__
#define __VIO_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vi.h"
#include "hi_comm_video.h"
#include "hi_comm_vo.h"
#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "hi_comm_aio.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "rmdebug.h"
#include "adchip.h"


void InitPlatform();
void VideoInit(tvsystem_t system, resolution_t resolution, int max_channel_num);
int StartVideoOut();
int ChangeVideoOut(outputmode_t outputmode, channelid_t channel[MAX_VIDEO_NUM]);

#endif	/*end of __vio_h__*/

