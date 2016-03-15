#include <stdio.h>
#include "chat.h"

void str_echo(int listenfd)
{
    int maxi, nready, i, n, connfd;
    struct pollfd clipolfd[OPEN_MAX];
    struct sockaddr_in cliaddr;
    struct chat_info cli_info;
    time_t ticks;
    memset(&cli_info, 0, sizeof(struct chat_info));
    for(i=0; i<OPEN_MAX; i++)
    {
        clipolfd[i].fd = -1;
    }

    clipolfd[0].fd     = listenfd;
    clipolfd[0].events = POLLRDNORM;

    maxi = 0;
    for(;;)
    {
        nready = Poll(clipolfd, maxi+1, INFTIM);
        for(i=0; i <= maxi; i++)
        {
            if(nready == 0)
            {
                break;
            }
            if( clipolfd[i].revents & (POLLRDNORM | POLLERR))
            {
                if(listenfd == clipolfd[i].fd)
                {
                    socklen_t len;
                    len = sizeof(cliaddr);
                    connfd = Accept(listenfd, (SA *)&cliaddr, &len);
                    printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                    nready--;
                    for(i=0; i<OPEN_MAX; i++)
                    {
                        if(clipolfd[i].fd == -1)
                        {
                            if(i>maxi)
                                maxi=i;
                            clipolfd[i].fd     = connfd;
                            clipolfd[i].events = POLLRDNORM;
                            break;
                        }
                    }

                }
                else
                {
                    nready--;
                    if( (n = Read(clipolfd[i].fd, &cli_info, sizeof(struct chat_info))) <= 0 )
                    {
                        if( (n < 0) && (errno == ECONNRESET) )
                        {
                            printf("client[%d] has aborted the connection\n", i);
                            continue;
                        }
                        else if(n < 0)
                            exit(1);
                        if(n == 0)
                        {
                            printf("client[%d] has terminted the connection\n", i);
                            close(clipolfd[i].fd);
                            clipolfd[i].fd = -1;
                            continue;
                        }
                    }

                    for(i=1; i<=maxi; i++)
                    {
                        if(clipolfd[i].fd != -1)
                        {

                            ticks = time(NULL);
                            snprintf(cli_info.RealTime, sizeof(cli_info.RealTime), "%.24s", ctime(&ticks));
                            //memcpy(cli_info.RealTime, ctime(time(NULL)), );
                            Writen(clipolfd[i].fd, &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
                        }
                    }
                    memset(&cli_info, 0, sizeof(struct chat_info));

                }
            }
        }
    }
}

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


