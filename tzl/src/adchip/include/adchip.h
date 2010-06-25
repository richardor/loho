#ifndef __ADCHIP_H__
#define __ADCHIP_H__

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
#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "hi_comm_aio.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "rmdebug.h"
#include "rmcommondatatype.h"
#include "tw2866.h"

#define ADCHIP_FILE_A	"/dev/tw2866dev"


void ADChipInit(tvsystem_t system);
void ADChipUninit();
int SetADChipWordMode(VI_WORK_MODE_E enWorkMode);


#endif /*__ADCHIP_H__*/

