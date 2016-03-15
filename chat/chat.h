#ifndef CHAT_H
#define CHAT_H

#include "zwunp.h"

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

#endif
