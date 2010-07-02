#include"fun_log.h"


int main()
{
   fun_log_set_level(LOG_DEBUG);
   fun_log_debug("%s 111111111111111\n","debug");
   fun_log_error("%s 222222222222222(%s)\n","error","error");


   fun_log_set_level(LOG_ERROR);
   fun_log_debug("%s 333333333333333\n","debug");
   fun_log_error("%s 444444444444444(%s)\n","error","error");
   return 0;	
}
