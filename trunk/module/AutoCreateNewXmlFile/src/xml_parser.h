#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__
#include"ini_parser.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _XmlParser XmlParser;

XmlParser *xml_parser_create(const char *xml_filename);

char *xml_parser_get_string(XmlParser *xml_parser, TagNode *tag_node);

void xml_parser_destroy(XmlParser *xmlParser);



#ifdef __cplusplus
}
#endif


#endif
