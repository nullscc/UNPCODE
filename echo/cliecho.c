#include "zwunp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void strcli_select(FILE* fp, int fd)
{
	char buf[MAXLINE];
	char echobuf[MAXLINE];
	fd_set sel_rdset;
	int maxfd;
	int stdineof = 0;
	
	FD_ZERO(&sel_rdset);
	for(;;)
	{
		FD_SET(fileno(fp), &sel_rdset);
		FD_SET(fd, &sel_rdset);
		maxfd = max(fileno(fp), fd) + 1;
		Select(maxfd, &sel_rdset, NULL, NULL, NULL);
		if(FD_ISSET(fd, &sel_rdset))
		{
			if( Read(fd, echobuf, MAXLINE) == 0)
			{
				if(stdineof == 0)
				{
					printf("server terminted prematurely\n");
				}
				return;
			}
			Fputs(echobuf, stdout);
			memset(echobuf, 0, MAXLINE);

		}
		if(FD_ISSET(fileno(fp), &sel_rdset))
		{
			if( Read(fileno(fp), buf, MAXLINE) == 0)
			{
					stdineof = 1;
					shutdown(fd, SHUT_WR);
					FD_CLR(fileno(fp), &sel_rdset);
					continue;
			}
			Writen(fd, buf, strlen(buf));
			memset(buf, 0, MAXLINE);
		}

	}

}


int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in srvaddr;
	
	if(argc != 2)
	{
		printf("Usage:%s <IPAdress>\n", argv[0]);
		return -1;
	}
	
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	inet_aton(argv[1], &srvaddr.sin_addr);

	Connect(sockfd, (SA*)&srvaddr, sizeof(srvaddr));

	strcli_select(stdin, sockfd);


	return 0;
}
