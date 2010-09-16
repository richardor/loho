#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/mount.h>
#include <sys/statfs.h>
#include"frame_manager.h"


#define return_val_if_fail(p, ret) if(!(p)) \
		{printf("%s:%d Warning: "#p" failed.\n",\
		__func__, __LINE__); return (ret);}



typedef struct  _FrameData
{
	datetime_t frame_time;
	bool i_frame;
}FrameData;


struct _FrameManager{
	unsigned int w_pos;
	unsigned int r_pos;
	unsigned int w_circuit_num;
	unsigned int r_circuit_num;	
	int length;
	datetime_t current_play_frame_time;
	bool pop_iframe_flag;
	FrameData *frame_data;
};


FrameManager *frame_manager_create(unsigned char frame_num)
{
	return_val_if_fail(frame_num < 100, NULL);

	FrameManager *thiz = NULL;
	
	thiz = (FrameManager *)malloc(sizeof(FrameManager));
	if(thiz != NULL)
	{
		thiz->w_pos = 0;
		thiz->r_pos = -1;
		thiz->w_circuit_num = 0;
		thiz->r_circuit_num = 0;
		thiz->length = frame_num;
		thiz->frame_data = (FrameData *)malloc(sizeof(FrameData) * frame_num);
		return_val_if_fail(thiz->frame_time != NULL, NULL);
		memset(thiz->frame_data, 0 ,sizeof(FrameData) * frame_num);
		pop_iframe_flag = false;
	}

	return thiz;
}


int frame_manager_add_frame(FrameManager *thiz, datetime_t date_time, bool i_frame);
{
	return_val_if_fail(thiz != NULL , -1);
	
	if(i_frame)
	{
		memcpy(&thiz->frame_data[thiz->w_pos]->frame_time, date_time, sizeof(datetime_t));
		thiz->frame_data[thiz->w_pos]->i_frame = true;
	}
	else
	{
		thiz->frame_data[thiz->w_pos]->i_frame = false;
	}
	
	thiz->w_pos++;
	if(thiz->w_pos >= thiz->length)
	{
		thiz->w_pos = 0;
		thiz->w_circuit_num +=1; 
	}
	return 0;
}

int frame_manager_pop_frame(FrameManager *thiz, unsigned int num)
{
	return_val_if_fail(thiz != NULL, -1);
	return_val_if_fail(num <= thiz->length, -1);
	int i = 0;

	for(i = 0; i < num; i++)
	{
		if(thiz->frame_data[thiz->r_pos]->i_frame)
		{
			memcpy(&thiz->current_play_frame_time, &thiz->frame_data[thiz->r_pos]->frame_time, sizeof(datetime_t));		
			if(pop_iframe_flag == false)
			{
				pop_iframe_flag = true;
			}
		}
		thiz->r_pos ++;
		if(thiz->r_pos >= thiz->length)
		{
			thiz->r_pos = 0;
			thiz->r_circuit_num ++;
		}
	}
	return 0;
}

int frame_manager_get_current_date_and_time(FrameManager *thiz, datetime_t *datetime)
{
	return_val_if_fail(thiz  && datetime, -1);

	if(pop_iframe_flag)
	{
		memcpy(datetime,&thiz->current_play_frame_time,sizeof(datetime_t));
		return 0;
	}
	return -1;	
}

unsigned int frame_manager_get_write_pos(FrameManager *thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->w_pos;
}


unsigned int frame_manager_get_write_circuit_num(FrameManager *thiz)
{
	return_val_if_fail(thiz != NULL, -1);
	return thiz->w_circuit_num;
}

unsigned int frame_manager_get_read_pos(FrameManager *thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->r_pos;
}

unsigned int frame_manager_get_read_circuit_num(FrameManager *thiz)
{
	return_val_if_fail(thiz != NULL, -1);
	return thiz->r_circuit_num;
}











