#include"frame_manager.h"


#define return_if_fail(p, ret) if(!(p)) \
		{printf("%s:%d Warning: "#p" failed.\n",\
		__func__, __LINE__); return ;}


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
		return_val_if_fail(thiz->frame_data != NULL, NULL);
		memset(thiz->frame_data, 0 ,sizeof(FrameData) * frame_num);
		thiz->pop_iframe_flag = false;
	}

	return thiz;
}


int frame_manager_add_frame(FrameManager *thiz, datetime_t date_time, bool i_frame)
{
	return_val_if_fail(thiz != NULL , -1);
	FrameData *frame_data_temp = NULL;

	frame_data_temp = thiz->frame_data + thiz->w_pos;
	if(i_frame)
	{
		memcpy(&(frame_data_temp->frame_time), &date_time, sizeof(datetime_t));
		frame_data_temp->i_frame = true;
		if(thiz->r_pos == -1)
		{
			thiz->r_pos = 0;
		}
	}
	else
	{
		frame_data_temp->i_frame = false;
	}
	
	thiz->w_pos++;
	if(thiz->w_pos >= thiz->length)
	{
		thiz->w_pos = 0;
		thiz->w_circuit_num++; 
	}
	return 0;
}

int frame_manager_pop_frame(FrameManager *thiz, unsigned int num)
{
	return_val_if_fail(thiz != NULL, -1);
	return_val_if_fail(num <= thiz->length, -1);

	FrameData *frame_data_temp = NULL;
	int i = 0;

	if(((thiz->w_pos + thiz->w_circuit_num * thiz->length) - (thiz->r_pos + thiz->r_circuit_num * thiz->length)) > thiz->length)
	{
		thiz->r_pos = (thiz->w_pos + thiz->w_circuit_num * thiz->length)%(thiz->length) + 1;
		thiz->r_circuit_num  = (thiz->w_pos + thiz->w_circuit_num * thiz->length - thiz->length)/thiz->length;
	}

	if(((thiz->w_pos + thiz->w_circuit_num * thiz->length) - (thiz->r_pos + thiz->r_circuit_num * thiz->length)) == 1)
	{
		frame_data_temp = thiz->frame_data + thiz->r_pos;
		if(frame_data_temp->i_frame)
		{
			memcpy(&thiz->current_play_frame_time, &frame_data_temp->frame_time, sizeof(datetime_t));		
			if(thiz->pop_iframe_flag == false)
			{
				thiz->pop_iframe_flag = true;
			}
		}
		return 0;
	}
	
	for(i = 0; i < num; i++)
	{
		frame_data_temp = thiz->frame_data + thiz->r_pos;
		if(frame_data_temp->i_frame)
		{
			memcpy(&thiz->current_play_frame_time, &frame_data_temp->frame_time, sizeof(datetime_t));		
			if(thiz->pop_iframe_flag == false)
			{
				thiz->pop_iframe_flag = true;
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

	if(thiz->pop_iframe_flag)
	{
		memcpy(datetime,&thiz->current_play_frame_time,sizeof(datetime_t));
		return 0;
	}
	return -1;	
}



void frame_manager_reset(FrameManager *thiz)
{
	return_if_fail(thiz && thiz->frame_data, -1);
	
	thiz->w_pos = 0;
	thiz->r_pos = -1;
	thiz->w_circuit_num = 0;
	thiz->r_circuit_num = 0;
	memset(thiz->frame_data, 0 ,sizeof(FrameData) * thiz->length);
	thiz->pop_iframe_flag = false;

	return;
}


void frame_manager_destory(FrameManager *thiz)
{
	return_if_fail(thiz, -1);
	
	if(thiz->frame_data)
	{
		free(thiz->frame_data);
		thiz->frame_data = NULL;
	}
	free(thiz);
	thiz = NULL;

	return ;
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











