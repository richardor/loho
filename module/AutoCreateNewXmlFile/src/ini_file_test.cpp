#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"ini_file.h"

void ini_database_print(ini_database_t *database)
{
	section_node_t *section_node_temp = NULL;
	data_node_t	*data_node_temp = NULL;
	if(database)
	{
		if(database->file_name)
		{
			printf("database->file_name(%s)\n",database->file_name);
		}
		section_node_temp = database->section_node;
		while(section_node_temp)
		{	
			if(section_node_temp->section_value)
			{
				printf("section_node_temp->section_node->section_value(%s)\n", section_node_temp->section_value);
			}
			data_node_temp = section_node_temp->data_node;
			
			while(data_node_temp)
			{	
				if(data_node_temp->data)
				{
					if(data_node_temp->data->id_value)
					{
						printf("id_value(%s)=",data_node_temp->data->id_value);
					}
					if(data_node_temp->data->string_value)
					{
						printf("string_value(%s)\n",data_node_temp->data->string_value);
					}
				}
				data_node_temp = data_node_temp->data_node_next;
			}
			section_node_temp = section_node_temp->section_node_next;
		}
		
	}
}


#define  INI_FILENAME  "english.ini"

int main(int argc , char *argv[])
{
	ini_database_t *database = NULL;

	InitGetLanguageString(INI_FILENAME);
	PrintIniDataBase();
	database = GetIniDatabase();	
	assert(database != NULL);
	ini_database_print(database);	


	return 0;
}
	
