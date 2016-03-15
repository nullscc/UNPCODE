#include "zwunp.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


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

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
	{
		perror("read error");
		return -1;
	}
	return(n);
}


int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int n;
	if( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
	{
		perror("select error");
		return -1;
	}
	return n;
}

int Poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
	int n;
	if( (n = poll(fds, nfds, timeout)) < 0)
	{
		perror("select error");
		return -1;
	}
	return n;
}

