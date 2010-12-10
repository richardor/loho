#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"public_function.h"




/*
 * create a thread
 */
Ret create_normal_thread(ThreadFunction function, void *para, pthread_t *pid)
{
	return_val_if_failed(para != NULL, RET_INVALID_PARAMETER);	
	
	pthread_t thread_id;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	if(pthread_create(&thread_id, &attr, function, para) == 0)
    {
        pthread_attr_destroy(&attr);
		if(pid != NULL)		
		{
			*pid = thread_id;
		}
        return RET_OK;
    }
	pthread_attr_destroy(&attr);
	
	return RET_FAILED;
}



