#ifndef __GET_LAN_STRING_H__
#define __GET_LAN_STRING_H__

#include<stdio.h>
#include "get_lan_string_typedef.h"



ini_database_t *GetLanguageStringCreate(language_e language);
char *GetLanguageString(char *section, char * id_value, char *default_str);
int UninitGetLanguageString();
void PrintIniDataBase(ini_database_t *ini_database);

void GetLanguageStringDestory(ini_database_t *ini_database);

#endif /*__GET_LAN_STRING_H__*/


