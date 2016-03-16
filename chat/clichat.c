#include "zwunp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "chat.h"

int main(int argc, char**argv)
{
	int sockfd;
	struct sockaddr_in srvaddr;
	struct chat_info cli_info;
	char option;
    int n;
    char loginresult[10];

	if(argc != 2)
	{
		printf("Usage:%s <IPAdress>\n", argv[0]);
		return -1;
	}
    memset(&cli_info, 0, sizeof(struct chat_info));

    printf("-------- Welcome Linux Terminal Chat Room --------\n");
    printf("Please Select Option Below:\n");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	inet_aton(argv[1], &srvaddr.sin_addr);

    Connect(sockfd, (SA*)&srvaddr, sizeof(srvaddr));

FIRST_IN:
    printf("--------------------\n");
    printf("| 1:Register       |\n");
    printf("| 2:Login          |\n");
    printf("--------------------\n");
    //read(fileno(stdin), &option, 1);
    option = getc(stdin);

    if(option == '1')
    {
        cli_info.flag = REGISTER;
        printf_flush("Please Input An User Name:");
        n = Read(fileno(stdin), cli_info.UserName, sizeof(cli_info.UserName));
        cli_info.UserName[n-1] = '\0'; //取消输入的'\n'

        printf_flush("Please Input A passwd:");
        n = Read(fileno(stdin), cli_info.UserPasswd, sizeof(cli_info.UserPasswd));
        cli_info.UserPasswd[n-1] = '\0'; //取消输入的'\n'
    }
    else if(option == '2')
    {
        cli_info.flag = LOGIN;
    }
    else
    {
        cli_info.flag = NONEOPTION;
        printf("Wrong Option,Please Select Option Below:\n");
        clearbuf();
        goto FIRST_IN;
    }

    if(cli_info.flag == REGISTER)
    {
        Writen(sockfd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
        //需要处理同名的情况
        printf("Register Success,Please Select Option Below:\n");
        clearbuf();
        goto FIRST_IN;
    }
    else if(cli_info.flag == LOGIN)
    {
        printf_flush("Your Name:");
        n = Read(fileno(stdin), cli_info.UserName, sizeof(cli_info.UserName));
        cli_info.UserName[n-1] = '\0'; //取消输入的'\n'

        printf_flush("Your Passwd:");
        n = Read(fileno(stdin), cli_info.UserPasswd, sizeof(cli_info.UserPasswd));
        cli_info.UserPasswd[n-1] = '\0'; //取消输入的'\n'
        n = Writen(sockfd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
        printf("client write %d byte to srv\n", n);
        n = Read(sockfd, loginresult, 10);
        printf("client read %d byte from srv\n", n);

        if(loginresult[0] == 'Y')
            DEBUG("login success\n");
        if(loginresult[0] == 'N')
            DEBUG("login fail\n");
        DEBUG("next is clearbuf\n");
        clearbuf();
        DEBUG("previous is clearbuf\n");
    }
    PRINTF_DESTINATION();
    cli_info.flag = SENDMSG;
    PRINTF_DESTINATION();
    clearbuf();

	strcli_select(stdin, sockfd, &cli_info);


	return 0;
}
