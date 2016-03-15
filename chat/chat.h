#ifndef CHAT_H
#define CHAT_H

#include "zwunp.h"
#include <time.h>

#define OPEN_MAX 256

enum Option
{
    NONEOPTION,
    REGISTER,
    LOGIN,
    SENDMSG,
    MAXOPTION
};

struct chat_info
{
	char UserName[25];
	char UserPassed[20];
	char RealTime[30];
    enum Option flag;
	char msg[MAXLINE];
};

void strcli_select(FILE* fp, int fd, struct chat_info *msginfo);

void str_echo(int listenfd);

#endif
