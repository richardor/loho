#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>


typedef struct _ListNode ListNode;

struct _ListNode{
	int value;
	ListNode *next;
};

#define return_if_fail(p) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n", \
		__func__, __LINE__); return;}
#define return_val_if_fail(p, ret) if(!(p)) \
	{printf("%s:%d Warning: "#p" failed.\n",\
	__func__, __LINE__); return (ret);}

typedef int (*Print)(void *ctx, void *data);


ListNode *list_create(int value);

int list_reverse(ListNode **list);
int list_insert(ListNode *list, int value);
int list_print(ListNode *list, Print vist);

int list_destory(ListNode *list);
#endif
