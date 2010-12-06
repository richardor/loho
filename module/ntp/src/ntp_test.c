#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "ntp.h"


int main(int argc , char *argv[])
{
	NtpClient *ntp_client = NULL;
	struct timeval tv;
	RetNtp	ret = RET_FAILED;
	int i = 0;

	printf("ntp test start!\n");
	ntp_client = ntp_client_create(NULL, -1);
	assert(ntp_client != NULL);
	for(i = 0; i < 10; i++)
	{
		ret = ntp_client_get_date_time(ntp_client, &tv);
		if(ret != RET_OK)		
		{
			printf("failed ret(%d)\n",ret);
		}
		printf("tv second(%d)\n", tv.tv_sec);
		settimeofday(&tv);	
	}

	ntp_client_destroy(ntp_client);
	printf("ntp client destroy!\n");

	return 0;
}
