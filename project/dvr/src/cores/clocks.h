#ifndef __CLOCKS_H__
#define __CLOCKS_H__
#include"public_typedef.h"
#include"observer.h"

DECLS_BEGIN

struct _Clocks;
typedef struct _Clocks Clocks;

struct _ClocksInfo;

typedef struct _ClocksInfo ClocksInfo;

typedef Ret (*ClocksGetTimeFunc)(Clocks *thiz, Datetime *date_time);
typedef Ret (*ClocksSetTimeFunc)(Clocks *thiz, Datetime *date_time);
typedef void (*ClocksDestroyFunc)(Clocks *thiz);

struct _Clocks
{
	ClocksSetTimeFunc set_time;
	ClocksGetTimeFunc get_time;
	ClocksDestroyFunc destroy;
	ClocksInfo *priv;
	void *priv_subclass[0];
};

static inline Ret clocks_get_time(Clocks *thiz, Datetime *date_time)
{
	return_val_if_failed(thiz!=NULL && date_time!=NULL && thiz->get_time!=NULL, RET_INVALID_PARAMETER);

	return thiz->get_time(thiz, date_time);
}

static inline Ret clocks_set_time(Clocks *thiz, Datetime *date_time)
{
	return_val_if_failed(thiz != NULL && date_time != NULL && thiz->get_time != NULL, RET_INVALID_PARAMETER);
	
	return thiz->set_time(thiz, date_time);
}

void clocks_init(Clocks *thiz);
Ret clocks_add_observer(Clocks *thiz, Observer *observer);
Ret clocks_remove_observer(Clocks *thiz, Observer *observer);
void clocks_new_second_observers_update(Clocks *thiz, Datetime *date_time);

void clocks_destory(Clocks *thiz);


DECLS_END


#endif
