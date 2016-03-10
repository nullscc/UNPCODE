#include "zwunp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

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

ssize_t writen(int fd, const void *buf, size_t count)
{
	ssize_t ntowrite;
	ssize_t n;
	const char *pbuf;

	ntowrite = count;
	pbuf = buf;
again:
	if( (n = write(fd, pbuf, ntowrite)) <= 0 )
	{
		if ( (errno == EINTR) && (n < 0) )
		{
			ntowrite -= n;
			pbuf += n;
			goto again;
		}
		else
			return -1;
	}
	return count;	
}


ssize_t Writen(int fd, const void *buf, size_t count)
{	
	if( writen(fd, buf, count) != count )
	{
		perror("writen error");
		return -1;
	}
	return count;
}


char *Fgets(char *s, int size, FILE *stream)
{
	char *str;
	if( (str = fgets(s, size, stream))==NULL )
		{
			perror("fgets error");
			return NULL;
		}
	return str;
}

int Fputs(const char *s, FILE *stream)
{
	int ret;
	if( (ret = fputs(s, stream))==EOF )
	{
		perror("fgets error");
		return -1;
	}
	return ret;
}

static int	read_cnt;
static char	*read_ptr;
static char	read_buf[MAXLINE];

static ssize_t
my_read(int fd, char *ptr)
{

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}


ssize_t Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
	{
		perror("readline error");
		return -1;
	}
	return(n);
}



