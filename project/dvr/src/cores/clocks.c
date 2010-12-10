#include<stdio.h>
#include"clocks.h"
#include"array_list.h"

struct _ClocksInfo 
{
	ArrayList *array_list;
};

void clocks_init(Clocks *thiz)
{
	return_if_failed(thiz != NULL && thiz->priv == NULL);

	thiz->priv = (ClocksInfo *)COMM_ZALLOC(sizeof(ClocksInfo));
	if(thiz->priv != NULL)
	{
		thiz->priv->array_list = array_list_create();
	}

	return;
}

Ret clocks_add_observer(Clocks *thiz, Observer *observer)
{	
	return_val_if_failed(thiz && observer, RET_INVALID_PARAMETER);		

	if(thiz->priv != NULL)
	{
		array_list_add(thiz->priv->array_list, observer);
	}
	
	return RET_OK;
}


Ret clocks_remove_observer(Clocks *thiz, Observer *observer)
{
	return_val_if_failed(thiz && observer, RET_INVALID_PARAMETER);	
		
	if(thiz->priv != NULL)
	{
		array_list_remove(thiz->priv->array_list, observer, sizeof(observer));	
	}

	return RET_OK;
} 

void clocks_destory(Clocks *thiz)
{
	return_if_failed(thiz != NULL);

	if(thiz->destroy)	
	{
		thiz->destroy(thiz);	
	}
	if(thiz->priv)
	{
		if(thiz->priv->array_list)
		{
			array_list_destroy(thiz->priv->array_list);
		}	
	}	
	COMM_ZFREE(thiz, sizeof(Clocks));

	return;
}
