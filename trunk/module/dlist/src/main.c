#include<stdio.h>
#include"dlist.h"


static RetDlist print_int(void *ctx, void *data)
{
	return_val_if_fail(ctx != NULL, RET_FAILED);
	
	printf("int value(%d)\n", (int *)ctx);
	return RET_OK;
}
static RetDlist print_string(void *ctx, void *data)
{
	return_val_if_fail(ctx != NULL, RET_FAILED);
	
	printf("int value(%s)\n", (int *)ctx);
	return RET_OK;
}
static RetDlist sum_cb(void *ctx, void *data)
{
	printf("sum_cb aaaaaaaaaaaa\n");
	long long *result = ctx;
	printf("sum_cb bbbbbb data(%d)\n", (int )data);
	*result += (int)data;
	
	return RET_OK;
}
static RetDlist cmp_cb(void *ctx, void *data)
{
	long long *result = ctx;
	printf("cmp_cb  data(%d) ctx(%d) aaaaaaaaaaaaaaaaaaaa\n", (int)data, *result);
	if(*result < (int)data)
	{
		printf("cmp_cb bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
		*result = (int)data;
	}
	return RET_OK;
}


int main(int argc , char *argv[])
{
	Dlist *dlist = NULL;
	int a = 0;
	char *string_a = "aaaaaaaaaaaaaaaaaaaaaaaa";
	char *string_b = "bbbbbbbbbbbbbbbbbbbbbbbb";
	int sum = 0;
	int max = 0;
	
	dlist = dlist_create();
	
	a = 1;
	dlist_insert(dlist, -1, (void *)a);
	a = 2;
	dlist_insert(dlist, -1, (void *)a);
	a = 3;
	dlist_insert(dlist, -1, (void *)a);
	a = 4;
	dlist_insert(dlist, -1, (void *)a);
	a = 5;
	dlist_insert(dlist, -1, (void *)a);
	
	dlist_print(dlist, print_int);

	dlist_destory(dlist);

	printf("########################################\n");
	dlist = dlist_create();
	dlist_insert(dlist, -1, (void *)string_a);
	dlist_insert(dlist, -1, (void *)string_b);
	dlist_print(dlist, print_string);
	dlist_destory(dlist);

	printf("########################################");
	printf("test sum all data\n");
	dlist = dlist_create();
	a = 1;
	dlist_insert(dlist, -1, (void *)a);
	a = 2;
	dlist_insert(dlist, -1, (void *)a);
	a = 3;
	dlist_insert(dlist, -1, (void *)a);
	a = 4;
	dlist_insert(dlist, 0, (void *)a);
	a = 5;
	dlist_insert(dlist, 66, (void *)a);

	dlist_foreach(dlist, sum_cb, &sum);
	printf("sum(%d)\n", sum);
	dlist_foreach(dlist, cmp_cb, &max);
	printf("max(%d)\n", max);
	dlist_destory(dlist);
	
	

	return 0;
}

