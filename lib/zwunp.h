#ifndef ZW_UNP_H
#define ZW_UNP_H



#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/poll.h>


#define	MAXLINE		4096	/* max text line length */

//typedef struct sockaddr SA;  // why error
#define SA struct sockaddr
typedef void Sigfunc(int);

#define	max(a,b)	((a) > (b) ? (a) : (b))

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t Writen(int fd, const void *buf, size_t count);

char *Fgets(char *s, int size, FILE *stream);

int Fputs(const char *s, FILE *stream);

ssize_t Read(int fd, void *ptr, size_t nbytes);

ssize_t Readline(int fd, void *ptr, size_t maxlen);

Sigfunc *Signal(int signo, Sigfunc *func);	/* for our signal() function */

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

int Poll(struct pollfd *fds, nfds_t nfds, int timeout);


#endif
