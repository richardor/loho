#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fly_engine.h"


static int  fly_engine_fly(FlyBehavior *thiz)
{
	printf("i believe i can fly with  the sky!\n");
	
	return 0;
}

static void fly_engine_destroy(FlyBehavior *thiz)
{
	if(thiz)
	{
		free(thiz);
		thiz == NULL;
	}
	return;
}

FlyBehavior *fly_engine_create()
{
	FlyBehavior *fly_behavior = (FlyBehavior *)malloc(sizeof(FlyBehavior));
	if(fly_behavior != NULL)
	{
		fly_behavior->fly = fly_engine_fly;
		fly_behavior->destroy = fly_engine_destroy;

		return fly_behavior;
	}
		
	return  NULL;
}


