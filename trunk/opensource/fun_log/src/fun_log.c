#include"fun_log.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>




FunLogLevel_t m_log_level = LOG_DEBUG;
FunLogWriteMode_t m_log_write_mode = LOG_PRINTF_MODE;
int m_write_log_fd = -1;


#define LOG_BUFFER_SIZE 1024

#define CONFIG_FILE "./log.conf"
#define CONFIG_LOG_WRITE_MODE "mode="
#define CONFIG_LOG_WRITE_LEVEL "level="

#define WRITE_LOG_FILE "log_file3.log"



int get_mode_level(const char*filename, FunLogWriteMode_t* mode, FunLogLevel_t* level)
{
	if(!filename)
	{
		printf("get_mode_level filename is null\n");
		return -1;	
	}
	if((mode == NULL) || (level == NULL))
	{
		printf("mode is null level is null\n");
		return -1;
	}
	
	FunLogWriteMode_t log_write_mode = LOG_PRINTF_MODE;
	FunLogLevel_t log_level = LOG_DEBUG;
 	FILE *fl = NULL;	
 	char buf_line[100];
 	char *p = NULL;
 	char *pKeyWord = NULL;
 	char *value = NULL;
 	int ret = -1;
 	
	/*open the config file*/
	fl = fopen(filename, "r");
	if(fl == NULL)
	{
		printf("fopen the file(%s)failed\n", CONFIG_FILE);
		goto OVER;
	}
	
	 /*get one line */
    if(fgets(buf_line, 100, fl) != NULL)
    {
    	printf("%s", buf_line);
    }
    p = buf_line;
    /*删掉换行符,在linux下换行符的值为0x0a*/
 	do
  	{  		
  		if(*p == 0x0a)
  		{
    		*p = '\0';
    		break;
  		}
  	}while(*p++);
  	
  	p = buf_line;
  	if(strstr(p, CONFIG_LOG_WRITE_MODE) != NULL)
  	{
  		value = p + strlen(CONFIG_LOG_WRITE_MODE);
  	}
  	
  	if(value != NULL)
  	{
  		printf("value(%s)\n",value);
  		if(strcmp(value, "0") == 0)
  		{
  			log_write_mode = LOG_PRINTF_MODE;
  		}
  		else if(strcmp(value, "1") == 0)
  		{
  			log_write_mode = LOG_FILE_MODE;
  		}
  		else if(strcmp(value, "2") == 0)
  		{
  			log_write_mode = LOG_SOCKET_MODE;
  		}
  	}
  	
	 /*get one line */
    if(fgets(buf_line, 100, fl) != NULL)
    {
    	printf("%s", buf_line);
    }
    p = buf_line;
    /*删掉换行符,在linux下换行符的值为0x0a*/
 	do
  	{  		
  		if(*p == 0x0a)
  		{
    		*p = '\0';
    		break;
  		}
  	}while(*p++);
  	
  	p = buf_line;
  	if(strstr(p, CONFIG_LOG_WRITE_LEVEL) != NULL)
  	{
  		value = p + strlen(CONFIG_LOG_WRITE_LEVEL);
  	}
  	
  	if(value != NULL)
  	{
  		printf("value(%s)\n",value);
  		if(strcmp(value, "0") == 0)
  		{
  			log_level = LOG_ERROR;
  		}
  		else if(strcmp(value, "1") == 0)
  		{
  			log_level = LOG_DEBUG;
  		}
  	}
  	  			
	
	ret = 0;
OVER:
	*mode = log_write_mode;
  	*level = log_level;
  	
	if(fl != NULL)
	{
		fclose(fl);
		fl = NULL;
	}
	
	return ret;
	
	
	
}


static int init_write_log_mode()
{
	int ret = -1;
	if(m_write_log_fd < 0)
	{
		m_write_log_fd = open(WRITE_LOG_FILE,O_RDWR|O_CREAT|O_APPEND);
		if(m_write_log_fd >= 0)
		{
			printf("open log file success!\n");
			ret = 0;
		}
		else
		{
			printf("open file failed(%s)\n");	
		}
			
	}	
	return ret;
	
}
static int write_log_to_file(char *buffer)
{
	printf("write_log_to_filem_write_log_fd(%d)\n", m_write_log_fd);
	if((buffer != NULL) && (m_write_log_fd >= 0))
	{
		printf("write_log_to_file buffer(%s), len(%d)", buffer, strlen(buffer));
		write(m_write_log_fd, buffer, strlen(buffer) + 1);
		return 0;	
	}	
	return -1;
}


static int fun_log(const char* format, va_list ap)
{
   	char buffer[LOG_BUFFER_SIZE] = {0};
	vsnprintf(buffer, LOG_BUFFER_SIZE, format, ap);
  
  	switch(m_log_write_mode)
  	{
  		case LOG_PRINTF_MODE:
			printf(buffer);
  			break;
  		case LOG_FILE_MODE:
  			write_log_to_file(buffer);
  			break;
  		case LOG_SOCKET_MODE:
  			break;
  		default:
  			break;
  	}
	return 0;
}




int fun_log_init()
{	
	if(get_mode_level(CONFIG_FILE, &m_log_write_mode, &m_log_level) != 0)
	{
		printf("fun_log_init failed!\n");
		return -1;	
	}
	
	switch(m_log_write_mode)
	{
		case LOG_FILE_MODE:
			init_write_log_mode();
			break;
		case LOG_SOCKET_MODE:
			break;
		default:
			break;	
	}
	
	return 0;
}

int fun_log_uninit()
{
	if(m_log_write_mode == LOG_FILE_MODE)
	{
		if(m_write_log_fd >= 0)
		{
			close(m_write_log_fd);
			m_write_log_fd = -1;	
		}

	}
	m_log_write_mode = LOG_INVALID_MODE;
	
	return 0;	
}

int fun_log_set_level(FunLogLevel_t level)
{
	
	  if((level != LOG_ERROR ) && (level != LOG_DEBUG))
	  {
	      printf("fun_log_set_level : Please input a valid parameter\n");
	      return -1;
	  }
	  else
	  {
	  	  m_log_level =  level;
	  }
	  return 0;	
}


int fun_log_debug(char *format, ...)
{
		va_list ap;
		va_start(ap, format);
	  
	  if(m_log_level >= LOG_DEBUG)
	  {
		    fun_log(format, ap);
	  }
		va_end(ap);

	  
	  return 0;	
}


int fun_log_error(char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	  
	if(m_log_level >= LOG_ERROR)
	{
		fun_log(format, ap);
	}
	va_end(ap);
    return 0;	
}
