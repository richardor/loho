#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"ini_file.h"



ini_database_t *m_ini_database = NULL;

static int DataBaseCreate()
{
	if(NULL == m_ini_database)
	{
		m_ini_database = (ini_database_t *)malloc(sizeof(ini_database_t));
		if(m_ini_database == NULL)
		{
			printf("m_ini_database malloc memory failed!\n");
			return -1;
		}
		m_ini_database->file_name = NULL;
		m_ini_database->section_node = NULL;
		m_ini_database->currrent_section_node = NULL;
	}
	return 0;
}

static char *ReadLine(char *line,   int	n,	 FILE	*stream) 
{ 
	static	 char*	 rs; 

	rs	 =	 fgets(line,   n,	stream);	
	if(strlen(line)	>= 1)   
	{ 
		line[strlen(line)	-	1]	 =	 '\0'; 
	}
	return	 rs; 
}

/*
	return 0  该 行有【】section标志
*/
static inline int IsSectionLine(char *line)
{
	int flag = -1;
	unsigned int i = 0;
	if(line)
	{	
		if((line[0] == '[') && (strlen(line) >=3))
		{
			for(i = 2; i < strlen(line); i++)
			{
				if(line[i] == ']')
				{
					flag = 0;
				}
			}	
		}
		return flag;
	}
	return -1;
}


/*
	return 0 该 行有= 标志，用于提取翻译文件
*/

static inline int GetIdAndString(char *line, char *id_value, char *string_value)
{
	if((line == NULL) || (id_value == NULL) || (string_value == NULL))
	{
		printf("GetIdAndString error : NULL parameter!\n");
		return -1;
	}
	
	unsigned int i = 0;

	for(i = 0; i < strlen(line); i++)
	{
		if(line[i] == '=')
		{
			break;
		}
	}
	if(i < strlen(line))
	{
		if(((strlen(line) - i - 1) < 256) && (i < 64))
		{
			memcpy(id_value, line, i);
			id_value[i + 1] = '\0';
			memcpy(string_value, &line[i + 1], strlen(line) - i - 1);
			string_value[strlen(line) - i] = '\0';
			return 0;	
		}
	}
	return -1;
}

static inline int GetSectionString(char *line,char *section, int len)
{	
	int start = 0;
	int end = 0;
	unsigned int i = 0;
	int section_length = 0;
	
	memset(section, 0 , len);
	if(line)
	{	
		for(i = 0; i < strlen(line); i++)
		{
			if(line[i] == '[')
			{
				start = i;
			}
			if(line[i] == ']')
			{
				end = i;
			}
		}
		if(end > 0)
		{
			section_length = end - start - 1;
			memcpy(section,&line[start + 1], section_length);
			section[section_length] = '\0';
		}
		return 0;
	}
	return -1;
}


static inline int InSertSectionNode(char *section_value)
{
	section_node_t *section_node_temp = NULL;
	if(section_value)
	{
		if(m_ini_database->section_node == NULL)
		{
			/*header insert*/
			m_ini_database->section_node = (section_node_t *)malloc(sizeof(section_node_t));
			if(m_ini_database->section_node == NULL)
			{
				printf(" InSertSectionNode malloc memory failed\n");
				return -1;
			}

			m_ini_database->currrent_section_node = m_ini_database->section_node;
			m_ini_database->section_node->data_node = NULL;
			m_ini_database->section_node->section_node_next = NULL; 	
			m_ini_database->section_node->section_value = strdup(section_value);
			
		}
		else
		{
			/*tail insert*/
			section_node_temp = (section_node_t *)malloc(sizeof(section_node_t));
			if(section_node_temp == NULL)
			{
				printf(" InSertSectionNode malloc memory failed\n");
				return -1;
			}
			section_node_temp->data_node = NULL;
			section_node_temp->section_node_next = NULL; 	
			section_node_temp->section_value = strdup(section_value);

			m_ini_database->currrent_section_node->section_node_next = section_node_temp;
			
			m_ini_database->currrent_section_node = m_ini_database->currrent_section_node->section_node_next;
			
		}
	}
	return -1;
}

static inline int InSertDataNode(char *id_value, char *string_value)
{
	if((!id_value) || (!string_value))
	{
		printf("InSertDataNode error null parameter!\n");
		return -1;
	}

	data_node_t *data_node_temp = NULL;
	if(m_ini_database->currrent_section_node == NULL)
	{
		printf("m_ini_database->currrent_section_node == NULL\n");
		return -1;
	}
	if(m_ini_database->currrent_section_node->data_node == NULL)
	{
		m_ini_database->currrent_section_node->data_node = (data_node_t *)malloc(sizeof(data_node_t));
		if(m_ini_database->currrent_section_node->data_node == NULL)
		{
			printf("malloc memory failed![m_ini_database->currrent_section_node->data_node]\n");
			return -1;
		}
		m_ini_database->currrent_section_node->data_node->data = (data_t *)malloc(sizeof(data_t));
		if(m_ini_database->currrent_section_node->data_node->data == NULL)
		{
			printf("malloc memory failed![m_ini_database->currrent_section_node->data_node->data]\n");
			return -1;
		}
		m_ini_database->currrent_section_node->data_node->data_node_next = NULL;

		m_ini_database->currrent_section_node->data_node->data->id_value = strdup(id_value);
		m_ini_database->currrent_section_node->data_node->data->string_value = strdup(string_value);
		m_ini_database->currrent_section_node->currrent_data_node = m_ini_database->currrent_section_node->data_node;
	}
	else
	{
		if(m_ini_database->currrent_section_node->currrent_data_node)
		{	
			data_node_temp = (data_node_t *)malloc(sizeof(data_node_t));
			if(data_node_temp == NULL)
			{
				printf("malloc memory failed![data_node_temp]\n");
				return -1;
			}
			data_node_temp->data = (data_t *)malloc(sizeof(data_t));
			if(data_node_temp->data == NULL)
			{
				printf("malloc memory failed![data_node_temp->data]\n");
				return -1;
			}
			data_node_temp->data_node_next = NULL;
			data_node_temp->data->id_value = strdup(id_value);
			data_node_temp->data->string_value = strdup(string_value);
			
			m_ini_database->currrent_section_node->currrent_data_node->data_node_next = data_node_temp;
			m_ini_database->currrent_section_node->currrent_data_node = data_node_temp;

		}		
		else
		{
			return -1;
		}
	}
	return 0;
}


static int LoadIniFile()
{
	FILE *file = NULL;
	char line_buf[256] = {0};
	char section_buf[64] = {0};
	char id_buf[64] = {0};
	char string_buf[256] = {0};
	
	/*1 open the file*/
	file = fopen(m_ini_database->file_name, "r");
	if(file == NULL)
	{
		printf("open file failed(%s)\n", m_ini_database->file_name);
		return -1;
	}

	while(!feof(file))
	{
		memset(line_buf, 0, 256);
		if(ReadLine(line_buf, 256, file) == NULL)
		{
			fclose(file);
			file = NULL;
			return -2;
		}
		/*find the section*/
		if(IsSectionLine(line_buf) == 0)
		{
			GetSectionString(line_buf, section_buf, 64);
			if(strlen(section_buf) > 0) 
			{
				InSertSectionNode(section_buf);	
			}
		}
		/*find the id  value*/
		if(GetIdAndString(line_buf, id_buf, string_buf) == 0)
		{
			if((strlen(id_buf) > 0)  && (strlen(string_buf) > 0))
			{
				InSertDataNode(id_buf, string_buf);
			}
			memset(id_buf, 0, 64);
			memset(string_buf, 0, 128);
		}
	}
	
	fclose(file);
	file = NULL;
	return 0;

}

int InitGetLanguageString(char *filename)
{
	if(!filename)
	{
		return  -1;
	}

	if(DataBaseCreate() != 0)
	{
		printf("DataBaseCreate failed!\n");
		return -1;
	}
	m_ini_database->file_name = strdup(filename);

	LoadIniFile();
	return 0;	
}


int UninitGetLanguageString()
{	
	section_node_t *cur_section_node = NULL;
	section_node_t *p_section_node = NULL;
	data_node_t *cur_data_node = NULL;
	data_node_t *p_data_node = NULL;
	if(m_ini_database)
	{
		FREE_MEMORY(m_ini_database->file_name);
		cur_section_node = m_ini_database->section_node;
		while(cur_section_node)
		{	
			p_section_node = cur_section_node;
			cur_data_node = cur_section_node->data_node;
			while(cur_data_node)
			{	
				p_data_node = cur_data_node;
				if(cur_data_node->data)
				{
					FREE_MEMORY(cur_data_node->data->id_value);
					FREE_MEMORY(cur_data_node->data->string_value);
				}
				cur_data_node = cur_data_node->data_node_next;
				FREE_MEMORY(p_data_node);
			}
			FREE_MEMORY(cur_section_node->section_value);
			//FREE_MEMORY(cur_section_node->data_node);
			cur_section_node = cur_section_node->section_node_next;
			FREE_MEMORY(p_section_node);
		}
		
		FREE_MEMORY(m_ini_database);
		return 0;
	}

	return -1;
}



char *GetLanguageString(char *section, char * id_value, char *default_str)
{
	if(m_ini_database == NULL)
	{
		printf("ini database is not initial\n");
		return NULL;
	}
	if((!section)|| (!id_value))
	{
		printf("section or idvlue is null\n");
		return NULL;
	}

	section_node_t *section_node_temp = NULL;
	data_node_t *data_node_temp = NULL;
	bool section_flag = false;
	bool find_flag = false;
	
	section_node_temp = m_ini_database->section_node;
	while(section_node_temp)
	{
		if(strcmp(section, section_node_temp->section_value) == 0)
		{
			section_flag = true;
			break;
		}
		section_node_temp = section_node_temp->section_node_next;
	}
	if(section_flag == true)
	{
		data_node_temp = section_node_temp->data_node;
		while(data_node_temp)
		{	
			if(data_node_temp->data && (data_node_temp->data->id_value) \
				&&(strcmp(data_node_temp->data->id_value, id_value) == 0))
			{
				find_flag = true;
				break;
			}
			data_node_temp = data_node_temp->data_node_next;
		}
	}

	if((find_flag) && (data_node_temp->data->string_value))
	{
		return data_node_temp->data->string_value;
	}
	return default_str;
}


int ChangeLanguageString(char *section, char * id_value, char *string)
{
	if(m_ini_database == NULL)
	{
		printf("ini database is not initial\n");
		return -1;
	}
	if((!section )|| (!id_value) || (!string))
	{
		printf("ChangeLanguageString invalied parameter input!\n");
		return -1;
	}
	
	section_node_t *section_node_temp = NULL;
	data_node_t *data_node_temp = NULL;
	bool section_flag = false;
	bool find_flag = false;
	int ret = -1;
	
	section_node_temp = m_ini_database->section_node;
	while(section_node_temp)
	{
		if(strcmp(section, section_node_temp->section_value) == 0)
		{
			section_flag = true;
			break;
		}
		section_node_temp = section_node_temp->section_node_next;
	}
	if(section_flag == true)
	{
		data_node_temp = section_node_temp->data_node;
		while(data_node_temp)
		{	
			if(data_node_temp->data && (data_node_temp->data->id_value) \
				&&(strcmp(data_node_temp->data->id_value, id_value) == 0))
			{
				find_flag = true;
				break;
			}
			data_node_temp = data_node_temp->data_node_next;
		}
	}

	if((find_flag) && (data_node_temp->data->string_value))
	{
		free(data_node_temp->data->string_value);
		data_node_temp->data->string_value = strdup(string);
		ret = 0;
	}
	return ret;
}


void PrintIniDataBase()
{
	section_node_t *section_node_temp = NULL;
	data_node_t	*data_node_temp = NULL;
	if(m_ini_database)
	{
		if(m_ini_database->file_name)
		{
			printf("m_ini_database->file_name(%s)\n",m_ini_database->file_name);
		}
		section_node_temp = m_ini_database->section_node;
		while(section_node_temp)
		{	
			if(section_node_temp->section_value)
			{
				printf("section_node_temp->section_node->section_value(%s)\n", section_node_temp->section_value);
			}
			data_node_temp = section_node_temp->data_node;
			
			while(data_node_temp)
			{	
				if(data_node_temp->data)
				{
					if(data_node_temp->data->id_value)
					{
						printf("id_value(%s)=",data_node_temp->data->id_value);
					}
					if(data_node_temp->data->string_value)
					{
						printf("string_value(%s)\n",data_node_temp->data->string_value);
					}
				}
				data_node_temp = data_node_temp->data_node_next;
			}
			section_node_temp = section_node_temp->section_node_next;
		}
		
	}
}


ini_database_t *GetIniDatabase()
{
	return m_ini_database;
}

