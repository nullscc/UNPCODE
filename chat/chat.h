#ifndef CHAT_H
#define CHAT_H

#include "zwunp.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OPEN_MAX 256

#define COLOR_NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

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

struct user_info
{
    struct sockaddr_in cliaddr;
    char cliname[25];
};

void strcli_select(FILE* fp, int fd, struct chat_info *msginfo);

void str_echo(int listenfd);

int file_exists(char *filename);

#endif
