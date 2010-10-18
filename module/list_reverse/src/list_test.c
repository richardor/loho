#include<stdio.h>
#include"list.h"


static int  PrintInt(void *ctx, void *data)
{
	printf("value (%d)\n", (int *)data);	
}

int main(int argc , char *argv[])
{
	ListNode *thiz = NULL;
	
	thiz = list_create(1);

	list_insert(thiz, 2);
	list_insert(thiz, 3);
	list_insert(thiz, 4);
	list_insert(thiz, 5);
	
	list_print(thiz, PrintInt);
	
	list_reverse(&thiz);
	list_print(thiz, PrintInt);
	
	list_destory(thiz);
	thiz = NULL;
	printf("destory the ListNode\n");	
	list_print(thiz, PrintInt);

	return 0;
}

