#include "zwunp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void strcli(FILE* fp, int fd)
{
	char buf[MAXLINE];
	char echobuf[MAXLINE];
	while( fgets(buf, MAXLINE, fp) != NULL)
	{
		write(fd, buf, strlen(buf));
		read(fd, echobuf, MAXLINE);
		fputs(echobuf, stdout);
	}
}

int main()
{
	int sockfd;
	struct sockaddr_in srvaddr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	inet_aton("127.0.0.1", &srvaddr.sin_addr);

	Connect(sockfd, (SA*)&srvaddr, sizeof(srvaddr));

	strcli(stdin, sockfd);


	return 0;
}
