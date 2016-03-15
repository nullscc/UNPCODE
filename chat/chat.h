#ifndef CHAT_H
#define CHAT_H

#include "zwunp.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    char UserPasswd[20];
	char RealTime[30];
    enum Option flag;
	char msg[MAXLINE];
};

void strcli_select(FILE* fp, int fd, struct chat_info *msginfo);

void str_echo(int listenfd);

int file_exists(char *filename);

#endif
