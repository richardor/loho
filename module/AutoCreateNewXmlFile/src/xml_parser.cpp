#include<stdio.h>
#include<stdlib.h>
#include"xml_parser.h"
#include"tinyxml.h"

struct _XmlParser{
	char *xml_filename;
	TiXmlDocument *doc;
};

/*
 * 
 */
XmlParser *xml_parser_create(const char *xml_filename)
{
	XmlParser *xml_parser = NULL;

	if(xml_filename)
	{
		xml_parser = (XmlParser *)malloc(sizeof(sizeof(XmlParser)));
		xml_parser->xml_filename = strdup(xml_filename);
		xml_parser->doc = new TiXmlDocument;
		if(xml_parser->doc->LoadFile(xml_filename) == false)
		{
			printf("error : load file failed!\n");
			return NULL;
		}	
	}
	return xml_parser;
}

/*
 *
 */
char *xml_parser_get_string(XmlParser *xml_parser, TagNode *tag_node)
{
	if(!xml_parser || !tag_node)
	{
		printf("error: invalid parameter!\n");
		return NULL;	
	}
	
	char *string = NULL;
	
	return string;
}

/*
 *
 */
void xml_parser_destroy(XmlParser *xml_parser)
{
	if(xml_parser)
	{
		if(xml_parser->xml_filename)
		{
			free(xml_parser->xml_filename);
			xml_parser->xml_filename = NULL;			
			delete xml_parser->doc;
			free(xml_parser);
			xml_parser = NULL;					
		}
	}
}



