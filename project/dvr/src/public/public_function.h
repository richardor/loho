#ifndef __PUBLIC_FUNCTION_H__
#define __PUBLIC_FUNCTION_H__
#include"public_typedef.h"

typedef void *(*ThreadFunction)(void *para);

DECLS_BEGIN

Ret create_normal_thread(ThreadFunction function, void *para, pthread_t *pid);

DECLS_END 


#endif/*__PUBLIC_FUNCTION_H__*/
