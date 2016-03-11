#include "zwunp.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int sig)
{
	pid_t pid;
	while( (pid = waitpid(-1, NULL, WNOHANG)) > 0 )
	{
		;//printf("child:%d has terminted\n", pid);
	}
	
	return;
}

void strecho(int fd)
{
	int n;
	char buf[MAXLINE];
again:
	while( (n = read(fd, buf, MAXLINE)) > 0)
	{	
		Writen(fd, buf, strlen(buf)); 
		bzero(buf, MAXLINE);
	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
	{
		perror("strecho: read error");
		return ;
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
	Signal(SIGCHLD, sig_chld);	
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

