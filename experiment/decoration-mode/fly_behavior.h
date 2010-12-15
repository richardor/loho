#ifndef __FLY_BEHAVIOR_H__
#define __FLY_BEHAVIOR_H__

struct _FlyBehavior;
typedef struct _FlyBehavior FlyBehavior;


typedef int (*FlyBehaviorFlyFunction)(FlyBehavior *thiz);
typedef void (*FlyBehaviorDestroyFunction)(FlyBehavior *thiz);

struct _FlyBehavior
{
	FlyBehaviorDestroyFunction destroy;
	FlyBehaviorFlyFunction fly;
	void *priv[0];
};


static  inline int  fly_behavior_fly(FlyBehavior *thiz)
{
	if(thiz == NULL  || thiz->fly == NULL)	
	{
		return -1;
	}

	return thiz->fly(thiz);
}

static inline void fly_behavior_destroy(FlyBehavior *thiz)
{
	if(thiz == NULL ||  thiz-> destroy == NULL)
	{
		return;
	}

	return thiz->destroy(thiz);
}



#endif /*__FLY_BEHAVIOR_H__*/
