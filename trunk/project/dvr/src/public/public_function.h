#ifndef __PUBLIC_FUNCTION_H__
#define __PUBLIC_FUNCTION_H__
#include"public_typedef.h"
#include<pthread.h>

typedef void *(*ThreadFunction)(void *para);

DECLS_BEGIN


Ret create_normal_thread(ThreadFunction function, void *para, pthread_t *pid);
void public_msleep(unsigned int milli_second);

DECLS_END 



#endif/*__PUBLIC_FUNCTION_H__*/
