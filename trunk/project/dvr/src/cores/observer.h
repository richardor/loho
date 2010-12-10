#ifndef __OBSERVER_H__
#define __OBSERVER_H__
#include"public_typedef.h"


DECLS_BEGIN

struct _Observer;
typedef struct _Observer Observer;

typedef Ret (*ObserverUpdateFunc)(Observer *thiz, void *data);
typedef void (*ObserverDestroyFunc)(Observer *thiz);

struct _Observer
{
	ObserverUpdateFunc update;
	ObserverDestroyFunc destroy;
	char priv[0];	
};

static inline Ret Observer_update(Observer *thiz, void *data)
{
	return_var_if_failed(thiz != NULL && data != NULL,RET_INVALID_PARAMETER);	

	return thiz->update(thiz, data);
}

static inline void Observer_destroy(Observer *thiz)
{
	return_if_failed((thiz!= NULL) && thiz->destroy);

	return thiz->destroy(thiz);
}

DECLS_END


#endif /*__OBSERVER_H__*/
