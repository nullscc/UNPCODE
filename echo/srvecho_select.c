#include <zwunp.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int listenfd, connfd;
	struct sockaddr_in srvaddr, cliaddr;
	int maxi, maxfd, nready, i, n, client[FD_SETSIZE];
	fd_set srvrdset;
	char buf[4096];
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	srvaddr.sin_addr.s_addr = INADDR_ANY;

	Bind(listenfd, (SA *)&srvaddr, sizeof(srvaddr));

	Listen(listenfd, 100);

	for(i=0; i<FD_SETSIZE; i++)
	{
		client[i] = -1;
	}
	client[0] = listenfd;
	maxi = 0;
	maxfd = listenfd;
	FD_ZERO(&srvrdset);
	for(;;)
	{
		for(i=0; i<=maxi; i++)
		{
			if(client[i] != -1)
				FD_SET(client[i], &srvrdset);
		}
		nready = Select(maxfd+1, &srvrdset, NULL, NULL, NULL);
		for(i=0; i <= maxi; i++)
		{
			if(nready == 0)
			{	
				break;
			}
			
			if( FD_ISSET(client[i], &srvrdset) )
			{
				if(listenfd == client[i])
				{
					socklen_t len;
					len = sizeof(cliaddr);
					connfd = Accept(listenfd, (SA *)&cliaddr, &len);
					printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
					nready--;
					if(connfd > maxfd)
						maxfd= connfd;
					for(i=0; i<FD_SETSIZE; i++)
					{
						if(client[i] == -1)
						{
							if(i>maxi)
								maxi=i;
							client[i] = connfd;
							break;
						}
					}
					
				}
				else
				{
					nready--;
					if( (n = Read(client[i], buf, 4096)) <= 0 )
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
							printf("client[%d] has terminted the connection\n", i);;
							FD_CLR(client[i], &srvrdset);
							close(client[i]);
							client[i] = -1;
							continue;
						}
					}
					for(i=1; i<=maxi; i++)
					{	
						if(client[i] != -1)
						Writen(client[i], buf, strlen(buf));
					}
					memset(buf, 0, 4096);
				}
			}
		}
	}
	
	return 0;
}
