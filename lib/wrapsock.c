#include "zwunp.h"

int Socket(int domain, int type, int protocol)
{
	int sockfd;
	if( (sockfd = socket(domain, type, protocol)) < 0  )
	{
		perror("create socketfd error");
		return -1;
	}
	return sockfd;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if( (ret =  bind(sockfd, addr, addrlen)) < 0 )
	{
		perror("bind error");
		return -1;
	}
	return ret;
}


int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	if( (ret =  Connect(sockfd, addr, addrlen)) < 0 )
	{
		perror("bind error");
		return -1;
	}
	
	return ret;
}

int Listen(int sockfd, int backlog)
{
	int ret;
	char	*ptr;

	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
	if( (ret =  Listen(sockfd, backlog)) < 0 )
	{
		perror("listen error");
		return -1;
	}

	return ret;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

{
	int connfd;

	if( (connfd =  accept(sockfd, addr, addrlen)) < 0 )
	{
		perror("listen error");

		//to be fixed;
		
		return -1;
	}

	return connfd;
}




