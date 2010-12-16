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
	void *priv[0];	
};

static inline Ret observer_update(Observer *thiz, void *data)
{
	return_val_if_failed(thiz != NULL && data != NULL,RET_INVALID_PARAMETER);	

	return thiz->update(thiz, data);
}

static inline void observer_destroy(Observer *thiz)
{
	return_if_failed((thiz!= NULL) && thiz->destroy);

	return thiz->destroy(thiz);
}

DECLS_END


#endif /*__OBSERVER_H__*/
