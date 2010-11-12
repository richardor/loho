#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"xml_parser.h"
#include"ini_parser.h"

#define XML_FILE_NAME	"2_36574E_4.xml"

int main(int argc, char *argv[])
{
	XmlParser *xml_parser = NULL;
	TagNode tag_node;
	memset(&tag_node, 0, sizeof(TagNode));
	char *string = NULL;	
//	char *xml_file = strdup("2_36574E_4.xml");	
	char xml_file[] = "2_36574E_4.xml";	
	

	tag_node.tag_type = TAG_HEAD;
	tag_node.tag_name = strdup("LoginPG");
	tag_node.key = strdup("INI_HED_LoginPG");
	
	xml_parser = xml_parser_create(xml_file);
	string = xml_parser_get_string(xml_parser, &tag_node);
	if(string)
	{
		printf("get the string(%s)\n", string);	
	}	
	
	return 0;
}
