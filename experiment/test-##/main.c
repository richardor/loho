#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int aa_bb = 2;


void funtest()
{
	printf("fun_test \n");

	return;
}

void fun_init()
{
	static bool do_init = false;

	if(do_init)
	{
		printf("init have do!\n");
		return;
	}	

	printf("fun init\n");
	do_init = true;

	return;
}

#define MACRO_FUN(a, b) a####b
#define USING(a) a##_init()

int main(int argc ,char *argv[])
{

	MACRO_FUN(fun, test)();

	//fun####test();

	USING(fun);
	USING(fun);

	return 0;
}
