#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include<time.h>
#include "ntp.h"

#define NTP_DEFAULT_IP  	"210.72.145.44"
#define NTP_DEFAULT_PORT	123

#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

#define JAN_1970 0x83aa7e80 /* 1900年～1970年之间的时间秒数 */
#define NTPFRAC(x)     (4294 * (x) + ((1981 * (x)) >> 11))
#define USEC(x)         (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))


#define NTP_PCK_LEN 48

typedef enum _NtpVersion{
	NTP_V1,
	NTP_V2,
	NTP_V3,
	NTP_V4
}NtpVersion;


struct _NtpClient{
	char *ntp_server_ip;
	int  ntp_server_port;
	NtpVersion ntp_version;
	struct addrinfo addr;
};


static int ntp_create_connect(NtpClient *ntp_client)
{
	if(!ntp_client)
	{
		printf("fun(%s), invalied parameter!\n", __FUNCTION__);
		return -1;
	}

	int ret = -1;
	int socket_fd = -1;
	struct addrinfo hints = {0};
	struct addrinfo  *res = NULL;
	char ntp_server_port[20] = {0};
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	snprintf(ntp_server_port, 20, "%d", ntp_client->ntp_server_port);
	ret = getaddrinfo(ntp_client->ntp_server_ip, ntp_server_port, &hints, &res);
	if(ret < 0)
	{
		printf("fun(%s), line(%d), getaddrinfo failed! ret(%d)\n", __FUNCTION__, __LINE__, ret);
		return 	-2;
	}

	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(socket_fd < 0)
	{		
		printf("create the socket failed! socket_fd(%d)\n", socket_fd);
		return -3;
	}

	memcpy(&ntp_client->addr, res, sizeof(struct addrinfo));

	return socket_fd;
}

static int ntp_construct_pack(NtpVersion ntp_version, char *packet)
{
	if(!packet)	
	{
		printf("fun(%s) : invalied parameter!\n", __func__);	
		return -1;
	}
	
	int version = 1;
	long tmp = 0;
	time_t timer = {0};
	
	/*get the version*/
	version = ntp_version + 1;
	tmp = htonl((LI << 30)|(version << 27)|(MODE << 24)|(STRATUM << 16)|(POLL << 8)|(PREC & 0xff));
	memcpy(packet, &tmp, sizeof(tmp));
	tmp = htonl(1<<16);	
	memcpy(&packet[4], &tmp, sizeof(tmp));
	memcpy(&packet[8], &tmp, sizeof(tmp));

	time(&timer);
	tmp = htonl(JAN_1970 + (long)timer);
	memcpy(&packet[40], &tmp, sizeof(tmp)); 
	tmp = htonl((long)NTPFRAC(timer));
	memcpy(&packet[44], &tmp, sizeof(tmp)); 

	return 0;
}

static int ntp_client_parser_packet_get_time(char *packet, struct timeval *ntp_time)
{	
	if(!packet && !ntp_time)
	{
		printf("fun(%s) : invalid parameter!\n", __func__);		
		return -1;
	}
	
	int second = 0;
	int usecond= 0;

	second = ntohl(*(int*)&(packet[40]));
	usecond = ntohl(*(int*)&(packet[44]));
	ntp_time->tv_sec = second - JAN_1970;
	ntp_time->tv_usec = USEC(usecond);	

	return 0;
}

static int ntp_get_ntp_time(NtpClient *ntp_client, int socket_fd, struct timeval *ntp_time)
{
	if(!ntp_client || !ntp_time || socket_fd < 0)
	{
		printf("fun(%s), invalied parameter!\n", __FUNCTION__);
		return -1;
	}

	fd_set fd;
	char data[NTP_PCK_LEN*8] = {0};
	int packet_len = 0;
	int ret = -1;
	int data_len = 0;
	struct timeval block_time = {0};
	int count  = 0;

    ntp_construct_pack(ntp_client->ntp_version, data);	
	
	data_len = ntp_client->addr.ai_addrlen;
	ret = sendto(socket_fd, data, NTP_PCK_LEN, 0, ntp_client->addr.ai_addr, data_len);
	if(ret < 0)
	{
		printf("fun(%s) : send to failed! ret(%d), socket_fd(%d)\n", __FUNCTION__, ret, socket_fd);
		return -1;	
	}
	
	FD_ZERO(&fd);
	FD_SET(socket_fd, &fd);
	block_time.tv_sec = 10;
	block_time.tv_usec = 0;
	
	printf("fun(%s) select\n", __func__);
	ret = select(socket_fd+1, &fd, NULL, NULL, &block_time);
	if(ret > 0)
	{
		printf("recv from!\n");
		count = recvfrom(socket_fd, data, NTP_PCK_LEN*8, 0, ntp_client->addr.ai_addr, &data_len);	
		if(count < 0)	
		{
			return -1;
		}	
	    ntp_client_parser_packet_get_time(data, ntp_time);
		printf("count(%d)\n", count);
	}

	return 0;
}

/*
* create the ntp client.
*/
NtpClient *ntp_client_create(char *ntp_server_ip, int ntp_server_port)
{
	NtpClient *client = NULL;	
	
	client = (NtpClient *)malloc(sizeof(NtpClient));
	memset(client, 0, sizeof(NtpClient));
	
	if(ntp_server_ip && ntp_server_port > 0)
	{
		client->ntp_server_ip = strdup("server_ip");
		client->ntp_server_port = ntp_server_port;
	}
	else
	{
		client->ntp_server_ip = strdup(NTP_DEFAULT_IP);	
        client->ntp_server_port = NTP_DEFAULT_PORT;
	}
    
	memset(&client->addr, 0 , sizeof(struct addrinfo));
	client->ntp_version = NTP_V3;

	return client;
}

/*
 * get the current date and time
 */
RetNtp ntp_client_get_date_time(NtpClient *ntp_client, struct timeval *ntp_time)
{
	if( !ntp_client || !ntp_time)
	{
		return RET_FAILED;
	}
	
	int socket_fd  = -1;
	RetNtp ret  = RET_FAILED;

	socket_fd = ntp_create_connect(ntp_client);
	if(socket_fd < 0)
	{
		ret = RET_CONNECT_FAILED;
		goto OVER;
	}

	printf("connect success! socket_fd(%d)\n", socket_fd);
	
	if(ntp_get_ntp_time(ntp_client, socket_fd,ntp_time) == 0)
	{
		ret = RET_OK;
	}

OVER:
	if(socket_fd >= 0)
	{
		close(socket_fd);
		socket_fd = -1;
	}
	return ret;
}
/*
 * destory the ntp client
 *
 */
void ntp_client_destroy(NtpClient *ntp_client)
{
	if(ntp_client)
	{	
		if(ntp_client->ntp_server_ip)
		{
			free(ntp_client->ntp_server_ip);		
			ntp_client->ntp_server_ip = NULL;
			free(ntp_client);
			ntp_client  = NULL;
		}
	}
}

