#include<stdio.h>
#include"get_lan_string.h"

//#include "get_lan_string_typedef.h"



int main(int argc ,char *argv[])
{
	char *test_string = NULL;
	char *section[]= {"google", "yahoo"};
	char *id_value[] = {"hello", "china", "byebye"};
	
	InitGetLanguageString(CHINESE_SIMPLE_LAN);

	test_string = GetLanguageString(section[0], id_value[0]);
	printf("section(%s), id_value(%s) string(%s)\n", section[0],id_value[0],test_string);
	test_string = GetLanguageString(section[0], id_value[1]);
	printf("section(%s), id_value(%s) string(%s)\n", section[0],id_value[1],test_string);
	test_string = GetLanguageString(section[1], id_value[2]);
	printf("section(%s), id_value(%s) string(%s)\n", section[1],id_value[2],test_string);

	
//	PrintIniDataBase();
	UninitGetLanguageString();

	return 0;
}
