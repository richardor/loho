#include<stdio.h>
#include<stdlib.h>
#include"fly_behavior.h"
#include"fly_engine.h"
#include"abstro_robot.h"

int main(int argc , char argv[])
{
	Robot *robot  = abstro_robot_create();	

	robot_fly(robot);

	return 0;
}



