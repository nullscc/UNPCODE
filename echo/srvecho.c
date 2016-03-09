#include "zwunp.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


void strecho(int fd)
{
	int n;
	char buf[MAXLINE];
	while( (n = read(fd, buf, MAXLINE)) > 0)
	{	
		write(fd, buf, strlen(buf)); 
		bzero(buf, MAXLINE);
	}

}

int main()
{
	int listenfd,connfd;
	struct sockaddr_in srvaddr;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	//inet_aton("127.0.0.1", &srvaddr.sin_addr);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd, (SA *)&srvaddr, sizeof(srvaddr));
	Listen(listenfd, 10);

	while( (connfd = Accept(listenfd, NULL, NULL)) > 0 )
	{
		if(0 == fork())
		{
			close(listenfd);
			strecho(connfd);
			exit(0);
		}
		else
		{
			close(connfd);
		}

	}


	return 0;
}

