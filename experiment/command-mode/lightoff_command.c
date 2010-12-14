#include<stdio.h>
#include<stdlib.h>
#include"lightoff_command.h"
#include"light.h"

typedef struct _PrivInfo{
	Light *light;
}PrivInfo;

static int lightoff_command_execute(Command *thiz)
{
	if(thiz == NULL)
	{
		return -1;
	}	
	PrivInfo *priv = NULL;
	
	priv = (PrivInfo *)thiz->priv0;

	if(priv != NULL && priv->light != NULL)
	{
		return light_off(priv->light);
	}

	return -1;
}

static void lightoff_command_destroy(Command *thiz)
{
	if(thiz)
	{
		if(thiz->priv0)
		{
			free(thiz->priv0);
		}
		free(thiz);
	}
	return;
}

Command *lightoff_command_create(Light *light)
{	
	if(light == NULL)
	{
		return NULL;
	}
	
	Command *command  = (Command *)malloc(sizeof(Command));	
	command->priv0 = (char *)malloc(sizeof(PrivInfo));
	
	if(command->priv0 != NULL)
	{
		PrivInfo *priv = NULL;	
		priv = (PrivInfo *)command->priv0;
		priv->light = light;
		command->execute = lightoff_command_execute;
		command->destroy = lightoff_command_destroy;
	}
	
	return command;

}
