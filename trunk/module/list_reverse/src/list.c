#include"list.h"
#include<stdio.h>
#include<stdlib.h>

ListNode *list_create(int value)
{
	ListNode *list = NULL;

	list = (ListNode *)malloc(sizeof(ListNode));
	return_val_if_fail(list, NULL);	
	
	list->value = value;
	list->next  = NULL;
	
	return list;
}

int list_destory(ListNode *list)
{
	return_val_if_fail(list, -1);
	
	ListNode *p = NULL;
	ListNode *next = NULL;	

	p = list;
	while(p)	
	{
		next = p->next;
		free(p);
		p = NULL;
		p = next;
	}

	return 0;

}

static ListNode * create_list_node(int value)
{
	ListNode *thiz = NULL;

	thiz= (ListNode*)malloc(sizeof(ListNode));
	return_val_if_fail(thiz, NULL);	
	thiz->value = value;
	thiz->next  = NULL;
	
	return thiz;
}


int list_insert(ListNode *list, int value)
{	
	return_val_if_fail(list, -1);
	
	ListNode *node = NULL;
	ListNode *p = NULL;
	ListNode *prev = NULL;

	p = list;
	node = create_list_node(value);	
	while(p)
	{
		prev = p;
		p = p->next;
	}
	prev->next  = node;

	return -1;
}

int list_print(ListNode *list, Print print)
{
	return_val_if_fail(list, -1);
	
	ListNode *p = NULL;
	p = list;
	
	while(p)
	{
		print(NULL,  (void *)p->value);
		p = p->next;
	}
	
	return 0;

}

int list_reverse(ListNode **list)
{
	return_val_if_fail(list, -1);
	
	ListNode *p = NULL;
	ListNode *tail = NULL;
	ListNode *tmp = NULL;
	
	p = *list;
	tail= *list;
	p = p->next;
	tail->next = NULL;	

	while(p)
	{
		tmp = p->next;
		p->next = tail;
		tail = p;
		p = tmp;
	}
	*list = tail;

	return 0;
}
