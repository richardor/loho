#include"fun_log.h"



FunLogLevel_t m_log_level = LOG_DEBUG;




int fun_log(const char* format, va_list ap)
{
   char buffer[LOG_BUFFER_SIZE] = {0};
   vsnprintf(buffer, LOG_BUFFER_SIZE, format, ap);
  
	  printf(buffer);
	
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
