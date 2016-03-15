#include "zwunp.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "chat.h"



int main()
{
    int listenfd;
    struct sockaddr_in srvaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	srvaddr.sin_addr.s_addr = INADDR_ANY;

	Bind(listenfd, (SA *)&srvaddr, sizeof(srvaddr));

	Listen(listenfd, 100);

    str_echo(listenfd);
	
	return 0;
}

