#include<stdio.h>
#include<stdlib.h>
#include"xml_parser.h"
#include"tinyxml.h"

struct _XmlParser{
	char *xml_filename;
	TiXmlDocument *doc;
};


#define GXKW_ROOT_TAG                   "_GUI_"
#define GXKW_EVENTLIST_TAG              "_EVENTLIST_"



static const char *get_node_attr_value(TiXmlElement *node, const char *attr)
{
	const char *string = NULL;

	if(node && attr)
	{
		string = node->Attribute(attr);		
	}

	return string;
}

static const char *get_conntent_comment(TiXmlElement *widget, TagNode *tag_node)
{
	if(!widget || !tag_node)
	{
		printf("fun(%s) line(%d): error invalid parameter!\n", __func__, __LINE__);
		return NULL;
	}

	const char *string  = NULL;
	TiXmlElement *ddl_item  = NULL;
	TiXmlElement *ddl_item_set  = NULL;
	int ddl_item_id = 0;	

	if(tag_node->tag_type == TAG_COMMENT)
	{
		string = get_node_attr_value(widget, "_COMMENT_");
	}
	else if(tag_node->tag_type == TAG_CONTENT)
	{		
		if(!tag_node->is_ddl)
		{
			string = get_node_attr_value(widget, "_CONTENT_");
		}
		else
		{
			ddl_item_set = widget->FirstChildElement("_ITEMLIST_");	
			for(ddl_item = ddl_item_set->FirstChildElement(); ddl_item != NULL; ddl_item = ddl_item->NextSiblingElement())
			{
				if(ddl_item_id == tag_node->ddl_id)
				{
					string = get_node_attr_value(ddl_item, "_CONTENT_");
				}
				ddl_item_id++;
			}
		}
	}

	return string;
}



static const char *parser_get_comment_content(TiXmlElement *page_node, TagNode *tag_node)
{
	if(!page_node || !tag_node)
	{
		printf("fun(%s) line(%d): error invalid parameter!\n", __func__, __LINE__);
		return NULL;
	}
	
	const char *string = NULL;
	const char *str = NULL;	
	TiXmlElement *page_child_node = NULL;
	TiXmlElement *widget_group = NULL;	
	TiXmlElement *widget = NULL;	
	
	for(page_child_node = page_node->FirstChildElement(); page_child_node != NULL; page_child_node = page_child_node->NextSiblingElement())	
	{
		str = page_child_node->Value();
		if(strcmp(str, GXKW_EVENTLIST_TAG) == 0)
		{
			continue;
		}
		for(widget_group = page_child_node; widget_group != NULL; widget_group = widget_group->NextSiblingElement())
		{
			for(widget = widget_group->FirstChildElement(); widget != NULL; widget = widget->NextSiblingElement())
			{
				str = get_node_attr_value(widget, "_NAME_");	
				if(str && strcmp(str, tag_node->tag_name) == 0)
				{
					string = get_conntent_comment(widget, tag_node);
					goto OVER;
				}
			}
		}
	}	

OVER:
	return string;
}


static const char *parser_get_string(TiXmlElement *page_node, TagNode *tag_node)
{
	if(!page_node || !tag_node)
	{
		printf("fun(%s) line(%d): error invalid parameter!\n", __func__, __LINE__);
		return NULL;
	}
	
	const char *string = NULL;
	const char *page_name = NULL;

	if(tag_node->tag_type == TAG_HEAD)	
	{
		for(;page_node != NULL; page_node = page_node->NextSiblingElement())
		{
			page_name  = get_node_attr_value(page_node, "_NAME_");
			if(page_name&&(strcmp(page_name, tag_node->tag_name) == 0))
			{
				string = get_node_attr_value(page_node, "_HEADER_");
				break;
			}
		}		
	}
	else if(tag_node->tag_type == TAG_COMMENT || tag_node->tag_type == TAG_CONTENT)
	{
		for(;page_node!= NULL;page_node = page_node->NextSiblingElement())
		{
			
			string = parser_get_comment_content(page_node,tag_node);
			if(string != NULL)
			{
				break;
			}				
		}
	}

	return string;
}

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

	const char *string = NULL;
	TiXmlElement *root_node  = NULL;
	TiXmlElement *osd_page_set_node  = NULL;
	TiXmlElement *first_osd_page_node = NULL;

	root_node= xml_parser->doc->FirstChildElement(GXKW_ROOT_TAG);
	if(root_node == NULL)
	{
		printf("error : root_node is null!\n");
		goto OVER;
	}
	osd_page_set_node = root_node->FirstChildElement();
	first_osd_page_node = osd_page_set_node->FirstChildElement();
	
	string  = parser_get_string(first_osd_page_node, tag_node);	

OVER:

	return (char *)string;
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



