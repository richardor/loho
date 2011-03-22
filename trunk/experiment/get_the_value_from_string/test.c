#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

bool is_valid_number(char ch)
{
	if(ch)
	{
		if((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
		{
			return true;
		}
	}
	return false;
}

int get_string_value(char *string, int bit)
{
 	if(string == NULL)
	{
		return -1;
	}
	
	char *p = string;
	
	if(strlen(p) < bit)
	{
		printf("error error .............\n");
		return -1;
	}

	p = p + bit - 1;
		
	if(is_valid_number(*p))
	{
		if(*p <= '9')
		{
			return (*p - '0');
		}
		else if(*p <= 'F')
		{
			return 10 + (*p - 'A');
		}
		else 
		{
			return 10 + (*p - 'a');
		}
	}
	return -1;

}	

int main(int argc, char *argv[])
{	
	int value = 0;

	int bit = 2;
	char *string = "123ABFabf";
	int i = 0;

	for(i = 1; i <= strlen(string); i++)
	{
		value = get_string_value(string, i);
		printf("string(%s) bit(%d)value(%d)\n", string, i,value);
	}

	return 0;
}

