/******************************************************************************
 * Filename : main.cpp
 * Copyright: Copyright sz
 * Created : 2010-11-15 by zhoumin
 * Description -
 * 
 *******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"ini_file.h"
#include"ini_parser.h"
#include"xml_parser.h"


static void usage()
{
	printf("usage : ./aut_create_new_xml ini_filename language_xml_filename\n");
}



struct _IniTagNodeCtx {
	char *id_string;
	TagNode *tag_node;
};


typedef  struct _IniTagNodeCtx IniTagNodeCtx;


int (*IniParserTagNode)(IniTagNodeCtx *tag_node_ctx, TagNode *tag_node)
{
	int ret = -1;

	if(tag_node->key)	
	
	if(strcmp(tag_node->key, tag_node_ctx->id_string) == 0)
	{
		tag_node_ctx->tag_node = tag_node;
		ret = 0;
	}
	return ret;
}


int main(int argc_node_temp = data_node_temp->data_node_next;
, char *argv[])
{
	if(argc < 3)	
	{
		usage();
		return -1;
	}
	
	ini_database_t *database = NULL;	
	Parser *ini_parser = NULL;
	XmlParser *xml_parser = NULL;	


	/*init  */
	InitGetLanguageString(argv[1]);
	database = GetIniDatabase();
	assert(database != NULL);
	ini_parser = ini_parser_create();
	ini_parser_init(ini_parser, argv[1]);
	xml_parser = xml_parser_create(argv[2]);

	/*foreach every node*/
	section_node_t *section_node_temp = NULL;
	data_node_t	*data_node_temp = NULL;

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

	return 0;
}

