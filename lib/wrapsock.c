#include "zwunp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

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
		if (errno == EPROTO || errno == ECONNABORTED || errno == EINTR)
#else
		if (errno == ECONNABORTED || errno == EINTR)
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

/*************************
 * 判断一个字符串是否有效IP的要点：
 * 1、只有数字和.
 * 2、连续的数字不能超过3个
 * 3、不能有连续的点，且点的个数有且必须是3个
 ************************/
int isvalidip(char *ip)
{
    int i;
    int m = 0;  //最大的连续的数字个数
    int n = 0;  //最大的连续的.的个数
    int k = 0;  //点的个数
    if( (ip == NULL)|| ( strlen(ip)<7 || strlen(ip)>15 ))
        return FALSE;
    for(i=0; i<16; i++)
    {
        if(ip[i] == '.')
        {
            n++;
            k++;
            if(n>2)
                return FALSE;
            m = 0;
        }
        else if( ip[i]>='0' && ip[i]<='9' )
        {
            m++;
            if(m>3)
                return FALSE;
            n = 0;
        }
        else
            return FALSE;
    }
    if(k != 3)
        return FALSE;
    return TRUE;

}
