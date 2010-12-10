#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _P{
	char *string;
}P;


int main(int argc, char *argv[])
{

	P *p1 = NULL;
	P *p2 = NULL;

	p1 = (P *)malloc(sizeof(P));	
	p1->string = strdup("hello world!");
	p2 = (P *)malloc(sizeof(P));	
	memcpy(p2, p1, sizeof(P));

	printf("11111111 p2->string %s\n", p2->string);
	
	
	p2->string = (char *)malloc(10);
	printf("22222222 p2->string %s\n", p2->string);

	p2->string = strdup("google!\n");
	printf("333333 p2->string %s\n", p2->string);

	return 0;

}
