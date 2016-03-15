#include <stdio.h>
#include "chat.h"
#include "zwunp.h"

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
            printf("%s\n", rcvinfo.RealTime);
            printf("%s:\n", rcvinfo.UserName);
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
