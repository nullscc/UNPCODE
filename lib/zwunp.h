#ifndef ZW_UNP_H
#define ZW_UNP_H



#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


#define	MAXLINE		4096	/* max text line length */

//typedef struct sockaddr SA;  // why error
#define SA struct sockaddr

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t Writen(int fd, const void *buf, size_t count);

char *Fgets(char *s, int size, FILE *stream);

int Fputs(const char *s, FILE *stream);


#endif
