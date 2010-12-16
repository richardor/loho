#include<stdio.h>
#include<stdlib.h>
#include"public_log.h"




int main(int argc, char *argv[])
{
	log_init(LOG_LEVEL_E, LOG_MODE_PRINT);
	
	log_v("log level is LOG_LEVEL_E version log\n");
	log_d("log level is LOG_LEVEL_E debug log\n");
	log_e("log level is LOG_LEVEL_E error log\n");

	log_init(LOG_LEVEL_D, LOG_MODE_PRINT);

	log_v("log level is LOG_LEVEL_D version log\n");
	log_d("log level is LOG_LEVEL_D debug log\n");
	log_e("log level is LOG_LEVEL_D error log\n");

	log_init(LOG_LEVEL_V, LOG_MODE_PRINT);

	log_v("log level is LOG_LEVEL_V version log\n");
	log_d("log level is LOG_LEVEL_V debug log\n");
	log_e("log level is LOG_LEVEL_V error log\n");

	log_init(LOG_LEVEL_V, LOG_MODE_FILE);

	log_v("log level is LOG_LEVEL_V version log\n");
	log_d("log level is LOG_LEVEL_V debug log\n");
	log_e("log level is LOG_LEVEL_V error log\n");


	return 0;
}

