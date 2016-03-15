#include "zwunp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "chat.h"

void strcli_select(FILE* fp, int fd, struct chat_info *msginfo)
{
	char buf[MAXLINE];
	struct chat_info rcvinfo;
	fd_set sel_rdset;
	int maxfd;
	int stdineof = 0;

	FD_ZERO(&sel_rdset);
	for(;;)
	{
		FD_SET(fileno(fp), &sel_rdset);
		FD_SET(fd, &sel_rdset);
		maxfd = max(fileno(fp), fd) + 1;
		Select(maxfd, &sel_rdset, NULL, NULL, NULL);
		if(FD_ISSET(fd, &sel_rdset))
		{
			if( Read(fd, &rcvinfo, sizeof(struct chat_info)) == 0)
			{
				if(stdineof == 0)
				{
					printf("server terminted prematurely\n");
				}
				return;
			}
			printf("%s      %s\n", rcvinfo.UserName, rcvinfo.RealTime);
			printf("%s\n", rcvinfo.msg);

			memset(&rcvinfo, 0, sizeof(struct chat_info));

		}
		if(FD_ISSET(fileno(fp), &sel_rdset))
		{
			if( Read(fileno(fp), buf, MAXLINE) == 0)
			{
					stdineof = 1;
					shutdown(fd, SHUT_WR);
					FD_CLR(fileno(fp), &sel_rdset);
					continue;
			}
			if( (buf[0] == '\n')  )
				continue;
			memset(msginfo->msg, 0, MAXLINE);
			memcpy(msginfo->msg, buf, strlen(buf));
			Writen(fd, msginfo, sizeof(struct chat_info) - (MAXLINE-strlen(buf)));
			memset(buf, 0, MAXLINE);
			memset(msginfo->msg, 0, MAXLINE);
		}

	}

}


int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in srvaddr;
	struct chat_info cli_info;
	char option;

	if(argc != 2)
	{
		printf("Usage:%s <IPAdress>\n", argv[0]);
		return -1;
	}
	printf("-------- Welcome Linux Terminal Chat Room --------\n");
	printf("Please Select Option Below\n");
	printf("--------------------\n");
	printf("| 1:Register       |\n");
	printf("| 2:Login          |\n");
	printf("--------------------\n");
    //read(fileno(stdin), &option, 1);
    option = getc(stdin);

	if(option == '1')
	{
        printf("Please Input An User Name:");
        fflush(stdout); //加上fflush强制刷新缓冲区
	}
	if(option == '2')
	{
		printf("Login\n");		
	}

	memset(&cli_info, 0, sizeof(struct chat_info));
	memcpy(cli_info.UserName, "baby", 25);

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	inet_aton(argv[1], &srvaddr.sin_addr);

	Connect(sockfd, (SA*)&srvaddr, sizeof(srvaddr));
    clearbuf();
	strcli_select(stdin, sockfd, &cli_info);


	return 0;
}
