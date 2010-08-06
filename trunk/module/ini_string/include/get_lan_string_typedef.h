#ifndef __GET_LAN_STRING_TYPEDEF_H__
#define __GET_LAN_STRING_TYPEDEF_H__


typedef struct _data_{
	char *id_value;
	char *string_value;
}data_t;


typedef struct _data_node_t data_node_t;
typedef struct _section_node_ section_node_t;


struct _data_node_t{
	data_t *data;
	data_node_t *data_node_next;
};


struct _section_node_{
	char *section_value;
	data_node_t *data_node;
	data_node_t *currrent_data_node;
	section_node_t *section_node_next;
};

typedef struct _ini_database_{
	char *file_name;	/*ini 文件名，根据语言的不同进行选 择。*/
	section_node_t *section_node;
	section_node_t *currrent_section_node;
}ini_database_t;





typedef enum _language_
{
	CHINESE_SIMPLE_LAN,
	ENGLISH_LAN,
	INVALID_LAN,
}language_e;



#define CHINESE_SIMPLE_INI	"./language/chinese_simple.ini"
#define ENGLISH_INIT		"./language/english.ini"

#define FREE_MEMORY(p)		if(p)\
							{\
								free(p);\
								p = NULL;\
							}





#endif /*__GET_LAN_STRING_H__*/

