#include<stdio.h>



void fun(void *ctx, void *data)
{
	long long *result = ctx;
	
	printf("*result(%d) *(int *)data(%d)\n", *result, *(int *)data);
	if(*result < *(int*)data)
	{
		printf("< less *result(%d)\n", *result);
	}
	if(*result == 0)
	{
		printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	}
}
int main()
{
	int b = 10000;
	int a = 1;
	fun(&a,&b);	

	return 0;
}
