#include "zwunp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void strcli(FILE* fp, int fd)
{
	char buf[MAXLINE];
	char echobuf[MAXLINE];

	while( Fgets(buf, MAXLINE, fp) != NULL)
	{
		Writen(fd, buf, strlen(buf));

		if(Readline(fd, echobuf, MAXLINE) == 0)
		{
			perror("read error:server terminted premature");
			return;
		}
		Fputs(echobuf, stdout);
		//printf("%s", echobuf);
		memset(echobuf, 0, MAXLINE);
		
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

	strcli(stdin, sockfd);


	return 0;
}
