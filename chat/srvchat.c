#include "zwunp.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "chat.h"

#define OPEN_MAX 256

int main()
{
	int listenfd, connfd;
	struct sockaddr_in srvaddr, cliaddr;
	int maxi, nready, i, n;
	struct pollfd clipolfd[OPEN_MAX];

	struct chat_info cli_info;
	time_t ticks;

	memset(&cli_info, 0, sizeof(struct chat_info));

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	srvaddr.sin_addr.s_addr = INADDR_ANY;

	Bind(listenfd, (SA *)&srvaddr, sizeof(srvaddr));

	Listen(listenfd, 100);

	for(i=0; i<OPEN_MAX; i++)
	{
		clipolfd[i].fd = -1;
	}
	
	clipolfd[0].fd     = listenfd;
	clipolfd[0].events = POLLRDNORM;
	
	maxi = 0;

	for(;;)
	{
		nready = Poll(clipolfd, maxi+1, INFTIM);
		for(i=0; i <= maxi; i++)
		{
			if(nready == 0)
			{	
				break;
			}
			if( clipolfd[i].revents & (POLLRDNORM | POLLERR))
			{
				if(listenfd == clipolfd[i].fd)
				{
					socklen_t len;
					len = sizeof(cliaddr);
					connfd = Accept(listenfd, (SA *)&cliaddr, &len);
					printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
					nready--;
					for(i=0; i<OPEN_MAX; i++)
					{
						if(clipolfd[i].fd == -1)
						{
							if(i>maxi)
								maxi=i;
							clipolfd[i].fd     = connfd;
							clipolfd[i].events = POLLRDNORM;
							break;
						}
					}
					
				}
				else
				{
					nready--;
					if( (n = Read(clipolfd[i].fd, &cli_info, sizeof(struct chat_info))) <= 0 )
					{
						if( (n < 0) && (errno == ECONNRESET) )
						{
							printf("client[%d] has aborted the connection\n", i);
							continue;
						}
						else if(n < 0)
							exit(1);
						if(n == 0)
						{
							printf("client[%d] has terminted the connection\n", i);
							close(clipolfd[i].fd);
							clipolfd[i].fd = -1;
							continue;
						}
					}

					for(i=1; i<=maxi; i++)
					{	
						if(clipolfd[i].fd != -1)
						{

							ticks = time(NULL);
							snprintf(cli_info.RealTime, sizeof(cli_info.RealTime), "%.24s", ctime(&ticks));
							//memcpy(cli_info.RealTime, ctime(time(NULL)), );
							Writen(clipolfd[i].fd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
						}
					}
					memset(&cli_info, 0, sizeof(struct chat_info));
					
				}
			}
		}
	}
	
	return 0;
}

