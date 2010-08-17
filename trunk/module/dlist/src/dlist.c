#include "dlist.h"





static DlistNode *dlist_get_node(Dlist *dlist, size_t index, int fail_return_last)
{
	DlistNode *iter = dlist->list_node;
	while(iter != NULL && iter->next != NULL && index > 0)
	{
		iter = iter->next;
		index--;
	}
	/*faile_return_last : 0 当没有找到时， 直接返回NULL
					  1  当没有找到时，返回最后一个iter*/
	if(!fail_return_last)
	{
		return index > 0?NULL:iter;
	}
	return iter;
}

static DlistNode *dlist_node_create(void *data)
{
	DlistNode *node = (DlistNode *)malloc(sizeof(DlistNode));

	if(node != NULL)
	{
		node->prev = NULL;
		node->next = NULL;
		node->data = data;
	}
	return node;
}


Dlist *dlist_create()
{
	Dlist *dlist = NULL;
	dlist = (Dlist *)malloc(sizeof(Dlist));
	return_val_if_fail(dlist != NULL, NULL);
	dlist->list_node = NULL;
	
	return dlist;
}


RetDlist dlist_insert(Dlist *dlist, size_t index, void *data)
{
	return_val_if_fail(dlist != NULL && data != NULL , RET_INVALIDPARAMETER);
	
	DlistNode *iter = NULL;
	DlistNode *cursor = NULL;
	int i = 0;
	
	/*1 create a data node*/
	iter = dlist_node_create(data);

	
	/*2 insert the data node to list!*/
	if(dlist->list_node == NULL)
	{
		dlist->list_node = iter;
		return RET_OK;
	}

	/*3 find the cursor node */
	cursor = dlist_get_node( dlist, index, 1);

	if(dlist->list_node == cursor)
	{
		dlist->list_node = iter;
	}
	else
	{
		cursor->prev->next = iter;
		iter->prev = cursor->prev;
	}
	iter->next = cursor;
	cursor->prev = iter;

	return RET_OK;
}


RetDlist dlist_header_insert(Dlist *dlist, void *data)
{
	
	return dlist_insert(dlist, 0, data);
}


RetDlist dlist_append(Dlist *dlist, void *data)
{
	return 	dlist_insert(dlist, -1, data);
}


RetDlist dlist_print(Dlist *dlist, DlistDataPrintFunc print)
{
	return_val_if_fail(dlist != NULL, RET_INVALIDPARAMETER);
	
	DlistNode *iter = NULL;

	iter = dlist->list_node;
	while(iter)
	{
		if(iter->data)
		{
			print(iter->data, NULL);	
		}
		iter = iter->next;
	}
	return RET_OK;
}

RetDlist dlist_foreach(Dlist *dlist, DlistDataVisitFunc visit, void *ctx)
{
	return_val_if_fail(dlist != NULL, RET_INVALIDPARAMETER);

	DlistNode *iter = NULL;

	iter = dlist->list_node;
	while(iter)
	{
		if(iter->data)
		{
			visit(ctx, iter->data);	
		}	
		iter = iter->next;
	}
	return RET_OK;
}

RetDlist dlist_destory(Dlist *dlist)
{	
	return_val_if_fail(dlist != NULL, RET_INVALIDPARAMETER);
	
	return RET_OK;
}



