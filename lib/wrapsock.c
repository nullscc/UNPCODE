#include "zwunp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int Socket(int domain, int type, int protocol)
{
	int sockfd;
	if( (sockfd = socket(domain, type, protocol)) < 0  )
	{
		perror("create socketfd error");
		exit(1);
	}
	return sockfd;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if( (ret =  bind(sockfd, addr, addrlen)) < 0 )
	{
		perror("bind error");
		exit(1);
	}
	return ret;
}


int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if( (ret =  connect(sockfd, addr, addrlen)) < 0 )
	{
		perror("connect error");
		exit(1);
	}
	
	return ret;
}

int Listen(int sockfd, int backlog)
{
	int ret;
	#if 0
	char	*ptr;

	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
	#endif
	if( (ret =  listen(sockfd, backlog)) < 0 )
	{
		perror("listen error");
		exit(1);
	}

	return ret;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

{
	int connfd;

	again:
	if ( (connfd = accept(sockfd, addr, addrlen)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
		{
			perror("accept error");
			exit(1);
		}
	}

	return connfd;
}




