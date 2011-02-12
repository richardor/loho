#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 


#define MAXLINE 80
#define SERV_PORT 8888

void do_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];
	
	//struct sockaddr_in  * cliaddr;
	
	for(;;)
	{
		len = clilen;
		/* waiting for receive data */
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		/* sent data back to client */
		sendto(sockfd, mesg, n, 0, pcliaddr, len);
		
		//cliaddr=(struct sockaddr_in *)pcliaddr;
		//printf("addr=%s",pcliaddr->sa_data);
		//printf("addr=%s--port=%d",inet_ntoa(cliaddr->sin_addr.s_addr),cliaddr->sin_port);
		//printf("addr=%s",pcliaddr->sa_data);
	}
}

int main(void)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */
	
	/* init servaddr */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	/* bind address and port to socket */
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}
	
	do_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	
	return 0;
}
