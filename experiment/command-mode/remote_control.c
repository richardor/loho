#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"remote_control.h"


struct _RemoteControl
{
	Command *on_command[5];
	Command *off_command[5];
	int slot_num;
};

#define MAX_SLOT_NUM 60

RemoteControl *remote_control_create(int slot_num)
{
	if(slot_num  < 0 || slot_num > MAX_SLOT_NUM)
	{
		return NULL;		
	}
	
	RemoteControl *remote_control = (RemoteControl *)malloc(sizeof(RemoteControl));
	int i = 0;
	
	if(remote_control != NULL)
	{
		remote_control->slot_num =  slot_num; 		
			
		return remote_control;
	}
	
	return remote_control;
}

void remote_control_set_command(RemoteControl *thiz, int slot, Command *on_command, Command *off_command)
{
	if(thiz == NULL)
	{
		return;
	}

	thiz->on_command[slot] =  on_command;	
	thiz->off_command[slot] = off_command;

	return;
}

void remote_control_on_button_press(RemoteControl *thiz, int slst)
{
	
	return;
}

void remote_control_off_button_press(RemoteControl *thiz, int slot)
{
	if(slot < 0 && slot > thiz->slot_num)
	{
		return;
	}

	printf("fun(%s) start\n", __func__);
	
	thiz->off_command[slot]->execute(thiz->off_command[slot]);

	return;
}

void *remote_control_destroy(RemoteControl *thiz)
{

	return;
}

