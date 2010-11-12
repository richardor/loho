#include<stdio.h>
#include<stdlib.h>
#include "ntp.h"


#define NTP_DEFAULT_IP				""
#define NTP_DEFAULT_PORT				


struct _NtpClient{
	char *ntp_server_ip;
	int  ntp_server_port;
	int connect_socket;
};


/*
* create the ntp client.
*/
NtpClient *ntp_client_create(char *ntp_server_ip, int ntp_server_port)
{
	NtpClient *client = NULL;	
	
	client = (NtpClient *)malloc(sizeof(NtpClient));
	memset(client, 0, sizeof(NtpClient));
	
	if(server_ip)
	{
		client->ntp_server_ip = strdup("server_ip");
	}
	else
	{
		client->ntp_server_ip = strdup(NTP_DEFAULT_IP);	
	}

    if(ntp_server_port > 0)
	{
        client->ntp_server_port = ntp_server_port;
	}
	else
	{
        client->ntp_server_port = NTP_DEFAULT_PORT;
	}
    
    client->connect_socket = -1;

	return client;
}


RetNtp ntp_client_set_server_ip(char *server_ip)
{
		
}

RetNtp ntp_client_get_date_time()
{


}

/*
 * destory the ntp client
 *
*/
void ntp_client_destor(NtpClient *ntp_client);
{
	if(ntp_client)
	{
        if(ntp_client->ntp_g)   
	}
}









