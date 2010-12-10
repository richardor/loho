#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"array_list.h"


int main(int argc , char *argv[])
{
	ArrayList *array_list = NULL;
	char *string1 = NULL;
	char *string2 = NULL;
	Ret ret;
	int id = -1;

	array_list = array_list_create();
	string1 = strdup("string1");
	string2 = strdup("string2");

	array_list_add(array_list, (void *)string1);
	array_list_add(array_list, (void *)string2);
	assert(array_list_get_size(array_list) == 2);

	id = array_list_get_id_by_data(array_list, string2, strlen(string2));
	assert(array_list_get_id_by_data(array_list, string1,strlen(string1)) == 0);
	assert(array_list_get_id_by_data(array_list, string2, strlen(string2)) == 1);
	assert(array_list_get_size(array_list) == 2);
	
	ret = array_list_remove(array_list, (void *)string1, strlen(string1));
	assert(ret == RET_OK);

	assert(array_list_get_size(array_list) == 1);
	
	array_list_destroy(array_list);

	return 0;

}
