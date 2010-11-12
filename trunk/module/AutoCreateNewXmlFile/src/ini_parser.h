/******************************************************************************
 * Filename : ini_parser.h
 * Copyright: Copyright sz
 * Created : 2010-11-12 by zhoumin
 * Description -
 * 
 *******************************************************************************/
#ifndef __INI_PARSER_H__
#define __INI_PARSER_H__



struct _Parser;
typedef struct _Parser Parser;

typedef enum _TagType{
	TAG_HEAD,
	TAG_CONTENT,
	TAG_COMMENT,
}TagType;

typedef struct  _TagNode{
	TagType tag_type;
	char *tag_name;
	int is_ddl;	
	int ddl_id;/*first ddl is 0*/
	char *key;
}TagNode;

typedef struct _IniTree IniTree;

struct _IniTree{
	TagNode *tag_node;
	IniTree *next;
};

typedef int (*IniParserTagNode)(void *ctx, void *data);


Parser *ini_parser_create();

int ini_parser_init(Parser *parser, char *ini_filename);
IniTree *ini_parser_get_ini_tree(Parser *parser);
int ini_parser_foreach(Parser *parser, IniParserTagNode visit, void *ctx);

void ini_parser_destroy(Parser *parser);


#endif /*end of __INI_PARSER_H__*/
