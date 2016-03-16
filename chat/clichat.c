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
    char result[10];

	if(argc != 2)
	{
		printf("Usage:%s <IPAdress>\n", argv[0]);
		return -1;
	}
    memset(&cli_info, 0, sizeof(struct chat_info));

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(6677);
	inet_aton(argv[1], &srvaddr.sin_addr);

    Connect(sockfd, (SA*)&srvaddr, sizeof(srvaddr));

    printf("-------- Welcome Linux Terminal Chat Room --------\n");
    printf("Please Select Option Below:\n");
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
        clearbuf(0);
        goto FIRST_IN;
    }

    if(cli_info.flag == REGISTER)
    {
        PRINTF_DESTINATION();
        Writen(sockfd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
        PRINTF_DESTINATION();
        //需要处理同名的情况
        n = Read(sockfd, result, 10);
        PRINTF_DESTINATION();
        clearbuf(1);
        PRINTF_DESTINATION();
        printf("n:%d, result:%s\n", n, result);
        if(result[0] == 'M')
        {
            printf("The User Name Has Already Be Register,Please Select Anothe Name:\n");
            clearbuf(0);
            goto FIRST_IN;
        }
        if(result[0] != 'Y')
        {
            printf("Register Success,Please Select Option Below:\n");
            clearbuf(0);
            goto FIRST_IN;
        }


    }
    else if(cli_info.flag == LOGIN)
    {
        printf_flush("Your Name:");
        n = Read(fileno(stdin), cli_info.UserName, sizeof(cli_info.UserName));
        cli_info.UserName[n-1] = '\0'; //取消输入的'\n'

        printf_flush("Your Passwd:");
        DEBUG_LONG("Read from input n=%d\n", n);
        n = Read(fileno(stdin), cli_info.UserPasswd, sizeof(cli_info.UserPasswd));
        DEBUG_LONG("Read from input n=%d\n", n);
        cli_info.UserPasswd[n-1] = '\0'; //取消输入的'\n'
        n = Writen(sockfd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
        DEBUG_LONG("writen to srv n=%d\n", n);
        n = Read(sockfd, result, 10);
        DEBUG_LONG("Read from srv n=%d\n", n);
        clearbuf(1);

        if(result[0] != 'Y')
        {
            printf("User Name or Passwd Incorrect,Please Retry Below:\n");
            clearbuf(0);
            goto FIRST_IN;
        }
        if(result[0] == 'Y')
        {
            printf("Login Success,You Could Send Message To You Want!\n");
        }

    }
    cli_info.flag = SENDMSG;
    clearbuf(1);

	strcli_select(stdin, sockfd, &cli_info);


	return 0;
}
