#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include"ini_parser.h"


struct _Parser{
	char *ini_filename;
	IniTree *ini_tree;
};

#define PRINTF(a) printf("fun(%s) line(%d) :%s", __func__, __LINE__, a)
#define MEMORY_MALLOC_CHECK(p) if(NULL == p){\
									PRINTF("memory malloc failed!\n");\
									exit(-1);\
								}	

#define MEMORY_FREE(p) if(p){\
							free(p);\
							p = NULL;\
						}

static void free_tag_node(TagNode *tag_node)
{
	if(tag_node)
	{	
		MEMORY_FREE(tag_node->tag_name);
		MEMORY_FREE(tag_node->key);
		MEMORY_FREE(tag_node);
	}
}

static char *read_file(char *filename)
{
	char *buffer = NULL;
	FILE *fp = NULL;

	if(filename)
	{
		struct stat st = {0};
		if(stat(filename, &st) == 0)
		{
			buffer = (char *)malloc(st.st_size + 1);
			fp = fopen(filename, "r");
			if(fp)
			{
				fread(buffer, st.st_size, 1, fp);
				buffer[st.st_size] = '\0';
			}
		}
	}	

	return buffer;
}


static void parse_key(char *key, char **widget_name, TagType *type, int *is_ddl, int *ddl_id)
{
	if(key)
	{
		PRINTF("start\n");
		enum _State{
			STAT_NONE = 0,
			STAT_TYPE, 
			STAT_NAME,
		}state = STAT_NONE;
		
		char *p = key;
		char *type_start = NULL;
		char *name_start = NULL;	

		
		for(p = key; *p != '\0'; p++)
		{
			switch(state)
			{
				case STAT_NONE:
					if( *p == '_')
					{
						type_start = p;
						state = STAT_TYPE;
					}
					break;
				case STAT_TYPE:
					if(*p == '_')
					{
						*p = '\0';
						name_start = p + 1;
						state = STAT_NAME;
					}
					break;				
				default:
					break;
			}
		}

		*is_ddl = 0;
		*widget_name = name_start;
		char ch; 
		if(strcmp(type_start, "_HED") == 0)
		{
			*type =  TAG_HEAD;
		}
		else if(strcmp(type_start, "_CON") == 0)
		{
			*type =  TAG_CONTENT;
			if(strstr(name_start, "DDL"))
			{
				*is_ddl = 1;
				ch = *(name_start + ((strlen(name_start)) - 1));  
				*ddl_id = ch - '1';			
				*(name_start + strlen(name_start)- 1) = '\0';
				*widget_name = name_start;
			}
		}
		else if(strcmp(type_start, "_COM") == 0)
		{
			*type  = TAG_COMMENT;
		}
		else
		{
			PRINTF("a valid key\n");
		}
	}
}

static TagNode *create_tree_node(TagType type, char *name, int is_ddl, int ddl_id, char *key)
{
	TagNode *node = NULL;

	if(name)
	{
		node = (TagNode *)malloc(sizeof(TagNode));
		MEMORY_MALLOC_CHECK(node);
		node->tag_type = type;
		node->tag_name = strdup(name);
		node->is_ddl = is_ddl;
		node->ddl_id = ddl_id;	
		printf("key(%s)\n", key);
		node->key = strdup(key);
	}	

	return node;
}

static void tree_printf(Parser *parser)
{
	if(parser)
	{
		IniTree *ini_tree = NULL;
		TagNode *tag_node = NULL;
		
		ini_tree = parser->ini_tree;
		while(ini_tree)
		{
			tag_node = ini_tree->tag_node;
			if(tag_node)
			{
				printf("type(%d),name(%s) is_ddl(%d), ddl_id(%d) key(%s)\n", tag_node->tag_type, tag_node->tag_name, tag_node->is_ddl, tag_node->ddl_id, tag_node->key);		
			}
			ini_tree = ini_tree->next;
		}			
	}	
}

static int parser_tree_insert_node(Parser *parser, TagNode *tag_node)
{
	if(!parser || !tag_node)
	{
		PRINTF("invalied parameter!\n");
		return -1;
	}

	IniTree *tree_head = NULL;
	IniTree *p = NULL;

	tree_head = parser->ini_tree;	
	
	if(NULL == tree_head)
	{
		tree_head = (IniTree *)malloc(sizeof(IniTree));	
		tree_head->tag_node = tag_node;	
		tree_head->next = NULL;
		parser->ini_tree = tree_head;
	}
	else
	{
		while(tree_head)	
		{
			p =  tree_head;		
			tree_head = tree_head->next;
		}	
		tree_head = (IniTree *)malloc(sizeof(IniTree));
		tree_head->tag_node = tag_node;
		tree_head->next = NULL;
		p->next = tree_head;		
	}
	tree_printf(parser);
	
	return 0;
}


static int parser_insert_node(Parser *parser, char *key, char *value)
{
	if(!key || !value)
	{
		PRINTF("invalid parameter!\n");
		return -1;
	}
	
	PRINTF("start!\n");
	char *widget_name = NULL;	
	TagType type = TAG_CONTENT;
	int is_ddl = 0;
	int ddl_id = 0;
	TagNode *node = NULL;
	char tmp_key_buf[32] = {0};
	
	memcpy(tmp_key_buf, key, strlen(key));
	parse_key(key, &widget_name, &type, &is_ddl, &ddl_id);
	PRINTF("\nafter parse key\n");
	printf("widget_name(%s) type(%d), is_ddl(%d), ddl_id(%d)\n", widget_name, type, is_ddl, ddl_id);
	node = create_tree_node(type, widget_name, is_ddl, ddl_id, tmp_key_buf);
	printf("after create_tree_node  type(%d), widget_name(%s), is_ddl(%d), ddl_id(%d)\n", node->tag_type, \
			node->tag_name, node->is_ddl, node->ddl_id);

	parser_tree_insert_node(parser, node);

	return 0;
}

static int ini_parse(Parser *parser, char *buffer)
{
	if(!buffer)
	{
		PRINTF("invalid parameter!\n");
		return -1;
	}
	
	PRINTF("start\n");
	char *key_start = NULL;	
	char *value_start = NULL;	
	char *p = buffer;
	enum _State { 
		STAT_NONE = 0,
		STAT_GROUP,
		STAT_KEY,
		STAT_VALUE,
		STAT_COMMENT,		
	}state = STAT_NONE;

	for( p = buffer; *p != '\0'; p++)
	{	
		switch(state)
		{
			case STAT_NONE:
				if(*p == '[')
				{
					state = STAT_GROUP;
				}
				else  if(!isspace(*p))
				{
					state = STAT_KEY;
					key_start = p;
				}
				break;
			case STAT_GROUP:
				if(*p == ']')
				{
					state = STAT_NONE;
				}
				break;
			case STAT_KEY:
				if(*p == '=')
				{
					*p = '\0';
					state = STAT_VALUE;
					value_start = p + 1;					
				}	
				break;
			case STAT_VALUE:
				if(*p == '\n' || *p == '\r')
				{
					*p = '\0';
					state = STAT_NONE;	
					printf("################key (%s) value(%s)\n", key_start, value_start);
					parser_insert_node(parser, key_start, value_start);
				}
				break;
			default:
				break;
		}
	}
	return 0;
}

/*
 *
 */
Parser *ini_parser_create()
{
	Parser *parser = NULL;
	
	parser = (Parser *)malloc(sizeof(Parser));
	parser->ini_filename = NULL;
	parser->ini_tree = NULL;

	return parser;

}

/*
 * 
 */
int ini_parser_init(Parser *parser, char *ini_filename)
{
	if(!parser || !ini_filename)
	{
		PRINTF("invalid parameter!\n");
		return -1;
	} 
	
	char *file_buffer = NULL;
	
	/*load the ini file*/
	PRINTF("load ini file\n");
	file_buffer = read_file(ini_filename);
	
	/*parser the ini file*/
	ini_parse(parser, file_buffer);

	
	return 0;
}

/*
 *
 */
IniTree *ini_parser_get_ini_tree(Parser *parser)
{
	IniTree *ini_tree = NULL;
	if(parser)
	{
		IniTree *ini_tree = NULL;
		ini_tree = parser->ini_tree;
	}
	return ini_tree;
}

/*
 *
 */
int ini_parser_foreach(Parser *parser, IniParserTagNode visit, void *ctx)
{
	if(!parser)
	{
		PRINTF("invalid parameter!\n");		
		return -1;
	}
	
	IniTree *ini_tree = NULL;
	TagNode *tag_node = NULL;

	ini_tree = parser->ini_tree;
	while(ini_tree)
	{
		tag_node = ini_tree->tag_node;
		if(visit(ctx, tag_node) == 0)
		{
			return 0;	
		}		
		ini_tree = ini_tree->next;
	}		

	return 0;	
}

/*
 *
 */
void ini_parser_destroy(Parser *parser)
{
	if(parser)
	{
		IniTree *ini_tree = NULL;
		IniTree *p = NULL;
		TagNode *tag_node = NULL;
		
		if(parser->ini_filename)
		{
			MEMORY_FREE(parser->ini_filename);
		}

		ini_tree = parser->ini_tree;	
		while(ini_tree)
		{
			p = ini_tree;
			tag_node = ini_tree->tag_node;
			free_tag_node(tag_node);
			MEMORY_FREE(ini_tree);
			ini_tree = p->next;
		}
		
		MEMORY_FREE(parser);	
	}	
}




