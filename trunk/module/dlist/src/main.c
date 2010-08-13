#include<stdio.h>
#include"dlist.h"


static RetDlist print_int(void *ctx, void *data)
{
	return_val_if_fail(ctx != NULL, RET_FAILED);
	
	printf("int value(%d)\n", (int *)ctx);
	return RET_OK;
}


int main(int argc , char *argv[])
{
	Dlist *dlist = NULL;
	int a = 0;
	
	dlist = dlist_create();
	
	a = 1;
	dlist_insert(dlist, -1, (void *)a);
	a = 2;
	dlist_insert(dlist, -1, (void *)a);

	dlist_print(dlist, print_int);

	dlist_destory(dlist);
	
	return 0;
}

