#include "dlist.h"




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
#if 0
	printf("dlist_insert aaaaaaaaaaaaaa\n");
	return_val_if_fail(dlist != NULL && data != NULL, RET_INVALIDPARAMETER);
	
	DlistNode *data_node_temp = NULL;
	DlistNode *data_node_pre = NULL;
	
	printf("dlist_insert bbbbbbbbbbbbbbbbbbbbbb\n");
	data_node_temp = dlist->list_node;
	data_node_pre = dlist->list_node;
	while(data_node_temp)
	{
		printf("dlist_insert cccccccccccccccccc\n");
		data_node_pre = data_node_temp;
		data_node_temp = data_node_temp->next;
	}
	printf("dlist_insert ddddddddddddddddddd\n"); 
	dlist->list_node = (DlistNode *)malloc(sizeof(DlistNode));
	dlist->list_node->prev = data_node_pre;
	dlist->list_node->next = NULL;
	dlist->list_node->data = data;
#endif
#if 1
	return_val_if_fail(dlist != NULL && data != NULL, RET_INVALIDPARAMETER);
	static int a = 0;
	if(a == 0)
	{
		printf("a(%d)\n",a);
		dlist->list_node = (DlistNode *)malloc(sizeof(DlistNode));
		
		dlist->list_node->data = data;
		dlist->list_node->prev = NULL;
		dlist->list_node->next = NULL;
		++a;
	}
	else
	{
		printf("data(%d)ggggggggggggggggggggggggggggggggggg a(%d) dlist->list_node->data(%d)\n",(int *)data, a, (int *)dlist->list_node->data);
		DlistNode *iter = NULL;
//		iter = dlist->list_node->next;
		iter = (DlistNode *)malloc(sizeof(DlistNode));
		iter->data = data;
		iter->prev = NULL;
		iter->next = NULL;
		dlist->list_node->next = iter;
		printf("ggggggggggggggggggggggggggggggggggg a(%d) dlist->list_node->data(%d) next->data(%d)\n", \
		a, (int *)dlist->list_node->data,(int *)dlist->list_node->next->data);
	}

#endif
	return RET_OK;
}

RetDlist dlist_append(Dlist *dlist, void *data)
{
	return 	dlist_insert(dlist, -1, data);
}


RetDlist dlist_print(Dlist *dlist, DlistDataPrintFunc print)
{
	printf("dlist_print aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	return_val_if_fail(dlist != NULL, RET_INVALIDPARAMETER);
	
	DlistNode *iter = NULL;

	iter = dlist->list_node;
	printf("dlist_print bbbbbbbbbbbbbbbbbbbbbb\n");
	while(iter)
	{
		printf("dlist_print cccccccccccccccccccc\n");
		if(iter->data)
		{
			printf("dlist_print ddddddddddddddddddddd\n");
			print(iter->data, NULL);	
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



