#include "zwunp.h"
#include<stdlib.h>
#include<stdio.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


int main()
{
	int listenfd;
	int Connfd; 
	char buff[100];
	struct sockaddr_in servaddr;
	time_t ticks;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	//inet_aton("127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	Listen(listenfd, 10);
	
	for( ; ;)
	{
		Connfd = Accept(listenfd, NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if( write(Connfd, &buff, strlen(buff))<0 )
			perror("write error");
		
		close(Connfd);
	}
	
	return 0;
}
