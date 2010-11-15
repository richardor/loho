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





struct _IniTagNodeCtx {
	char *id_string;
	TagNode *tag_node;
};

typedef  struct _IniTagNodeCtx IniTagNodeCtx;


int IniParserTagNode(IniTagNodeCtx *tag_node_ctx, TagNode *tag_node)
{
	int ret = -1;
	
	if(tag_node && (strcmp(tag_node->key, tag_node_ctx->id_string) == 0))
	{
		tag_node_ctx->tag_node = tag_node;
		ret = 0;
	}
	return ret;
}

static void usage()
{
	printf("usage : ./aut_create_new_xml ini_filename language_xml_filename\n");
}

static void create_new_xml_file(ini_database_t *ini_database, int fd)
{
	section_node_t *section_node_temp = NULL;
	data_node_t	*data_node_temp = NULL;
	char buffer[128] = {0};	

	section_node_temp = database->section_node;

	while(section_node_temp)
	{	
		if(section_node_temp->section_value)
		{
			printf("section_node_temp->section_node->section_value(%s)\n", section_node_temp->section_value);
			sprintf(buffer, "[%s]\r\n",  section_node_temp->section_value);
			write(fd, buffer, strlen(buffer));
		}
		data_node_temp = section_node_temp->data_node;
	
		while(data_node_temp)
		{	
			if(data_node_temp->data)
			{
				if(data_node_temp->data->id_value)
				{
					sprintf(buffer, "%=%s\r\n", data_node_temp->data->id_value, data_node_temp->data->string_value);
					write(fd, buffer, strlen(buffer));
				}
			}
			data_node_temp = data_node_temp->data_node_next;
		}
		section_node_temp = section_node_temp->section_node_next;
	}

}


int main(int argc, char *argv[])
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
	IniTagNodeCtx ctx_ini_tag_node;
	section_node_temp = database->section_node;
	char *string_transaltor = NULL;
	int fd  = -1;
	

	
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
					ctx_ini_tag_node->id_value = data_node_temo_data->id_value;
					ini_parser_foreach(ini_parser, IniParserTagNode, (void *)&ctx_ini_tag_node);
					string_transaltor = xml_parser_get_string(xml_parser, ctx_ini_tag_node.tag_node);			
					if(data_node_temp->dat->string_value && string_transaltor)
					{
						free(data_node_temp->string_value);
						data_node_temp->string_value = strdup(string_transaltor);
					}
				}
			}
			data_node_temp = data_node_temp->data_node_next;
		}
		section_node_temp = section_node_temp->section_node_next;
	}

	/*creat the new ini file*/
	fd = open("new_xml_file.xml", O_RDWR | CREAT );
	create_new_xml_file(ini_database, fd);
		
	/*destroy*/
	if(fd > 0)
	{
		close(fd);
		fd = -1;
	}
	ini_parser_destroy(ini_parser);
	xml_parser_destroy(xml_parser);
	UninitGetLanguageString();
	return 0;
}

