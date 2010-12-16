#include<stdio.h>
#include<stdlib.h>
#include"array_list.h"

struct _List;
typedef struct _List List;

struct _List{
	List *next;
	void *data;
};

struct _ArrayList
{
	int array_list_number; 
	List *header;
};

static List *list_node_create(void *data)
{
	return_val_if_failed(data, NULL);
	
	List *list = (List *)COMM_ZALLOC(sizeof(List));
	
	list->data = data;
	list->next = NULL;

	return list;	
}

ArrayList *array_list_create()
{
	ArrayList *array_list = NULL;
	array_list = (ArrayList *)COMM_ZALLOC(sizeof(ArrayList));
	
	return array_list;
}

Ret array_list_add(ArrayList *thiz, void *data)
{
	return_val_if_failed(thiz && data, RET_INVALID_PARAMETER);

	List *p = NULL;	
	List *current = NULL;	

	p = thiz->header;
	if(p == NULL)
	{
		thiz->header = list_node_create(data);
	}
	else
	{
		while(p)
		{
			current = p;
			p = p->next;
		}
		p = list_node_create(data);	
		current->next = p;
	}

	thiz->array_list_number++;

	return RET_OK;
}

Ret array_list_remove(ArrayList *thiz, void *data, int data_size)
{
	return_val_if_failed(thiz && data && data_size > 0, RET_INVALID_PARAMETER);

	List *p = thiz->header;

	List *prev = p;
	while(p)
	{
		if((p->data) && (memcmp(p->data, data, data_size) == 0))
		{
			if( p == thiz->header)
			{
				p = NULL;
			}
			else
			{
				prev->next = p->next;
			}
			thiz->array_list_number--;
			return RET_OK;
		}
		prev = p;
		p = p->next;		
	}
	return RET_FAILED;
}	

int array_list_get_id_by_data(ArrayList *thiz, void *data, int data_size)
{		
	return_val_if_failed(thiz && data && data_size>0, -1);		
	
	if(thiz->array_list_number == 0)
	{
		printf("fun(%s): error \n", __func__);
		return -1;
	}
	
	List *p = thiz->header;
	int i = -1;

	while(p)
	{
		++i;
		if((p->data) && (memcmp(p->data, data, data_size) == 0))
		{
			return i;
		}
		p = p->next;
	}
	
	printf("fun(%s), out\n", __func__);
	return -1;
}

void *array_list_get_data_by_id(ArrayList *thiz, int id)
{
	return_val_if_failed(thiz&&(id>=0), NULL);
	
	int i = 0;	
	List *p = NULL;
	
	p = thiz->header;
	for(i = 0; i <= id; i++)
	{		
		if(p && p->data && (i == id))
		{
			return p->data;
		}
		p = p->next;	
	}
	
	return NULL;
}

int array_list_get_size(ArrayList *thiz)
{
	return_val_if_failed(thiz, -1);

	return thiz->array_list_number;
}

void array_list_destroy(ArrayList *thiz)
{
	return_if_failed(thiz);	
	
	List *p = NULL;
	List *prev = NULL;	

	p = thiz->header;
	while(p)
	{
		prev = p;
		p = p->next;
		if(prev->data)
		{
			COMM_FREE(prev->data);
		}
		COMM_FREE(prev);
	}
	COMM_FREE(thiz);
}

