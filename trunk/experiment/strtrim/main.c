#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc ,char *argv[])
{	
	char *string = "hello=world!";
	
	printf("string(%s)\n", strtrim(string));

	return 0;
}
