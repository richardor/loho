#ifndef __NTP_H__
#define __NTP_H__

#include<stdio.h> 

typedef _Ret{
	RET_OK,
	RET_FAILED,
	RET_CONNECT_FAILED,
	RET_GET_VALIED_VALUE, 
	RET_BUTT,
}RetNtp;

typedef _NtpClient NtpClient;

NtpClient *ntp_client_create(char *server_name);

RetNtp ntp_client_set_server_ip();

RetNtp ntp_client_get_date_time();

void ntp_client_destroy(NtpClient *ntp_client);





#endif /*end of __NTP_H__*/
