#ifndef __NTP_H__
#define __NTP_H__

#include<stdio.h> 

typedef enum _RetNtp{
	RET_OK,
	RET_FAILED,
	RET_CONNECT_FAILED,
	RET_GET_VALIED_VALUE, 
	RET_BUTT,
}RetNtp;

typedef struct _NtpClient NtpClient;

#ifdef	__cplusplus
extern "C" {
#endif

NtpClient *ntp_client_create(char *ntp_server_ip, int ntp_server_port);
RetNtp ntp_client_get_date_time(NtpClient *ntp_client, struct timeval *ntp_time);
void ntp_client_destroy(NtpClient *ntp_client);

#ifdef	__cplusplus
}
#endif



#endif /*end of __NTP_H__*/
