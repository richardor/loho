#ifndef __GET_LAN_STRING_TYPEDEF_H__
#define __GET_LAN_STRING_TYPEDEF_H__




typedef enum _language_
{
	CHINESE_SIMPLE_LAN,
	ENGLISH_LAN,
	INVALID_LAN,
}language_e;



#define CHINESE_SIMPLE_INI	"chinese_simple.ini"
#define ENGLISH_INIT		"english.ini"

#define FREE_MEMORY(p)		if(p)\
							{\
								free(p);\
								p = NULL;\
							}





#endif /*__GET_LAN_STRING_H__*/

