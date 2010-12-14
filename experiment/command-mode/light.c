#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"light.h"


struct _Light
{
	char *light_name;
};

Light *light_create(char *light_name)
{
	Light *light = NULL;
	if(light_name != NULL)
	{
		light  = (Light *)malloc(sizeof(Light));	
		light->light_name  = (char *)malloc(strlen(light_name) + 1);
		memcpy(light->light_name, light_name, strlen(light_name) + 1);
	}
	return light;
}


int light_on(Light *thiz)
{
	if(thiz == NULL)
	{
		return -1;
	}

	printf("%s light_on\n", thiz->light_name);
	
	return 0;
}

int light_off(Light *thiz)
{
	if(thiz == NULL)
	{
		return -1;
	}

	printf("%s light_off\n", thiz->light_name);
	
	return 0;
}


void  *light_destroy(Light *thiz)
{
	return;
}
