#include<stdio.h>
#include<stdlib.h>
#include"abstro_robot.h"
#include"fly_engine.h"

Robot *abstro_robot_create()
{
	Robot *thiz = NULL;
	
	thiz = (Robot *)malloc(sizeof(Robot));

	thiz->fly_behavior =  fly_engine_create();

	return thiz;
}

