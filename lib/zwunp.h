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
#include <errno.h>
#include <stdlib.h>

#define COLOR_NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

#define PRINTF_RED(msg, arg...) \
    //printf("\033[1;31m" "%s %s(%d) " msg "\033[m\n", __FILE__, __FUNCTION__, __LINE__ , ##arg)

#define PRINTF_DESTINATION() \
    //printf("\033[1;31m" "%s %s(%d) " "\033[m\n", __FILE__, __FUNCTION__, __LINE__)

#define DEBUG_LONG(msg, arg...) \
    //printf( "%s %s(%d) " msg , __FILE__, __FUNCTION__, __LINE__ , ##arg)

#define DEBUG(msg, arg...) \
    //printf( msg , ##arg)

#define	MAXLINE		4096	/* max text line length */

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


#define container_of(ptr, type, member) ({      \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})

//typedef struct sockaddr SA;  // why error
#define SA struct sockaddr
typedef void Sigfunc(int);

#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif

#define	max(a,b)	((a) > (b) ? (a) : (b))

#define TRUE 1
#define FALSE 0

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

void clearbuf(int flag);

void printf_flush(char * const str);

#endif
