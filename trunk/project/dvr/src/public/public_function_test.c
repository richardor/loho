#include<stdio.h>
#include<stdlib.h>
#include"public_function.h"




void *thread_test(void *para)
{
	while(1)
	{
		printf("test_test!\n");
		sleep(1);
	}
	return NULL;
}


void test_thread()
{
	int temp;	

	create_normal_thread(thread_test, (void *)&temp, NULL);
	
	printf("input 'q' to quit\n");
	while(getchar() != 'q')
	{
		printf("input 'q' to quit\n");
		sleep(1);
	}

	return;
}


int main(int argc ,char *argv[])
{
	printf("public function test begin\n");	

	test_thread();

	printf("public function test over\n");

	return 0;
}
