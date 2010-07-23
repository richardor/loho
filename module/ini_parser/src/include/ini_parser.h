#ifndef __GET_LAN_STRING_H__
#define __GET_LAN_STRING_H__

#include<stdio.h>
#include "get_lan_string_typedef.h"



int ini_parser_create(char *file_name);
char *ini_parser_get_string(char *section, char * id_value);
int ini_parser_destory();
void PrintIniDataBase();



#endif /*__GET_LAN_STRING_H__*/

