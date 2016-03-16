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

    printf(LIGHT_PURPLE"-------- Welcome Linux Terminal Chat Room --------\n"COLOR_NONE);
    printf(LIGHT_CYAN"Please Select Option Below:\n"COLOR_NONE);
FIRST_IN:
    printf(LIGHT_CYAN"--------------------\n"COLOR_NONE);
    printf(LIGHT_CYAN"| 1:Register       |\n"COLOR_NONE);
    printf(LIGHT_CYAN"| 2:Login          |\n"COLOR_NONE);
    printf(LIGHT_CYAN"--------------------\n"COLOR_NONE);
    //read(fileno(stdin), &option, 1);
    option = getc(stdin);

    if(option == '1')
    {
        cli_info.flag = REGISTER;
        printf_flush(LIGHT_CYAN"Please Input An User Name:"COLOR_NONE);
        n = Read(fileno(stdin), cli_info.UserName, sizeof(cli_info.UserName));
        cli_info.UserName[n-1] = '\0'; //取消输入的'\n'

        printf_flush(LIGHT_CYAN"Please Input A passwd:"COLOR_NONE);
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
        printf(LIGHT_RED"Wrong Option,Please Select Option Below:\n"COLOR_NONE);
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
        if(result[0] == 'M')
        {
            printf(LIGHT_RED"The User Name Has Already Be Register,Please Select Anothe Name:\n"COLOR_NONE);
            clearbuf(0);
            goto FIRST_IN;
        }
        if(result[0] != 'Y')
        {
            printf(LIGHT_RED"Register Success,Please Select Option Below:\n"COLOR_NONE);
            clearbuf(0);
            goto FIRST_IN;
        }


    }
    else if(cli_info.flag == LOGIN)
    {
        printf_flush(LIGHT_CYAN"Your Name:"COLOR_NONE);
        n = Read(fileno(stdin), cli_info.UserName, sizeof(cli_info.UserName));
        cli_info.UserName[n-1] = '\0'; //取消输入的'\n'

        printf_flush(LIGHT_CYAN"Your Passwd:"COLOR_NONE);
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
            printf(LIGHT_RED"User Name or Passwd Incorrect,Please Retry Below:\n"COLOR_NONE);
            clearbuf(0);
            goto FIRST_IN;
        }
        if(result[0] == 'Y')
        {
            printf(LIGHT_CYAN"Login Success,You Could Send Message To You Want!\n"LIGHT_CYAN);
        }

    }
    cli_info.flag = SENDMSG;
    clearbuf(1);

	strcli_select(stdin, sockfd, &cli_info);


	return 0;
}
