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

void clocks_new_second_observers_update(Clocks *thiz, Datetime *datetime)
{
	if(thiz)
	{
		ArrayList *array_list = thiz->priv->array_list;
		int array_list_number = 0;
		int i = 0;
		Observer *observer = NULL;

		if(array_list != NULL)
		{
			array_list_number = array_list_get_size(array_list);
			for(i = 0; i < array_list_number; i++)
			{
				observer =(Observer *)array_list_get_data_by_id(array_list, i);				
				if(observer != NULL)
				{
					observer_update(observer,(void *)datetime);
				}
			}
		}
	}
}

void clocks_destory(Clocks *thiz)
{
	return_if_failed(thiz != NULL);

	if(thiz->priv)
	{
		if(thiz->priv->array_list)
		{
			array_list_destroy(thiz->priv->array_list);
		}	
	}

	if(thiz->destroy)	
	{
		thiz->destroy(thiz);	
	}
	
	COMM_ZFREE(thiz, sizeof(Clocks));

	return;
}
