#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"ini_parser.h"

#define INI_XML_FILENAME "english.ini"



static void ini_tree_printf(IniTree *ini_tree)
{
	if(ini_tree)
	{
		IniTree *head = ini_tree;
		while(head)
		{
			printf("type(%d),name(%s), is_ddl(%d), ddl_id(%d)\n", head->tag_node->tag_type, head->tag_node->tag_name, head->tag_node->is_ddl, head->tag_node->ddl_id);		
			head = head->next;
		}	
	
	}
}

int main(int argc, char *argv[])
{
	Parser *parser = NULL;
	IniTree *ini_tree = NULL;	

	parser = ini_parser_create();
	assert(parser != NULL);
	ini_parser_init(parser, INI_XML_FILENAME);
	ini_tree = ini_parser_get_ini_tree(parser);
//	assert(ini_tree != NULL);
	//ini_tree_printf(ini_tree);	
	
	return 0;
}
