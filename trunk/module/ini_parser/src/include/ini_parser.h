#ifndef __GET_LAN_STRING_H__
#define __GET_LAN_STRING_H__

#include<stdio.h>
#include "get_lan_string_typedef.h"



int ini_parser_create(char *file_name);
char *GetLanguageString(char *section, char * id_value);
int UninitGetLanguageString();
void PrintIniDataBase();



#endif /*__GET_LAN_STRING_H__*/

