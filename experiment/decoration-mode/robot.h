#ifndef __ROBOT_H__
#define __ROBOT_H__
#include"fly_behavior.h"

struct _Robot;
typedef struct _Robot Robot;


typedef void (*RobotDestroy)(Robot *thiz);

struct _Robot
{
	FlyBehavior *fly_behavior;
	RobotDestroy destroy;
	void *priv[0];
};


static inline int robot_fly(Robot *thiz)
{
	if(thiz == NULL || thiz->fly_behavior == NULL)
	{
		return -1;
	}
	
	thiz->fly_behavior->fly(thiz->fly_behavior);

	return  0;
}


static inline void  robot_destroy(Robot *thiz)
{
	if(thiz == NULL || thiz->destroy == NULL)
	{
		return ;
	}

	return thiz->destroy(thiz);
}





#endif /*__ROBOT_H__*/
