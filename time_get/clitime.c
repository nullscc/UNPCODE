#include "zwunp.h"
#include<stdlib.h>
#include<stdio.h>
#include <strings.h>
#include <unistd.h>

#define ENABLE_BIND_LOCAL_PORT 1

int main(int argc, char **argv)
{
		int sock_fd, n;
		char recieve[100];
		struct sockaddr_in serv_addr;
#if ENABLE_BIND_LOCAL_PORT
		struct sockaddr_in local_addr;
#endif
#if ENABLE_BIND_LOCAL_PORT
	if( argc != 4)
	{
		printf("Usage:%s <Remote IPaddress> <local IPaddress> <local port>\n", argv[0]);
		return -1;
	}
#else
		if( argc != 2)
		{
			printf("Usage:%s <IPaddress>\n", argv[0]);
			return -1;
		}
#endif
		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(13);
		//inet_aton("127.0.0.1", &serv_addr.sin_addr);
		inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
		
#if ENABLE_BIND_LOCAL_PORT
		bzero(&local_addr, sizeof(local_addr));
		local_addr.sin_family = AF_INET;
		local_addr.sin_port = htons(atoi(argv[3]));
		inet_pton(AF_INET, argv[2], &local_addr.sin_addr);
#endif

		sock_fd = Socket(AF_INET, SOCK_STREAM, 0);
		
#if ENABLE_BIND_LOCAL_PORT
		Bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
#endif
		Connect(sock_fd, (const struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
		
		while((n = read(sock_fd, recieve, 100)) > 0)
		{
			recieve[n] = 0;
			if(fputs(recieve, stdout) == EOF)	
			{
				perror("error fputs");	
			}
		}
		
		if(n<0)
		{
				perror("read error");
		}
		return 0;
}
