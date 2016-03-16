#include <stdio.h>
#include "chat.h"
#include "zwunp.h"

void reg_to_passwd_file(struct chat_info *info, char *filename)
{
    int passwd_fd;
    char buf[100];
    DEBUG_LONG("excute reg_to_passwd_file\n");
    passwd_fd = open(filename, O_CREAT|O_APPEND|O_RDWR);
    if(passwd_fd < 0)
    {
        perror("open or create /etc/chat.passwd failed");
        exit(1);
    }
    if(lseek(passwd_fd, 0, SEEK_END) < 0)
    {
        perror("lseek failed");
        exit(1);
    }
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(info->UserName) + sizeof(info->UserPasswd) + 1, "%s:%s\n", info->UserName, info->UserPasswd);
    Writen(passwd_fd, buf, strlen(buf));
    close(passwd_fd);
}

void handle_login(struct chat_info *info, char *filename, int *login_flag, int fdindex, int sockfd)
{
    int passwd_fd;
    char buf[100];
    char bufname[50];
    char bufpasswd[50];
    char loginresult;
    int n;
    DEBUG_LONG("excute handle_login\n");
    passwd_fd = open(filename, O_RDONLY);
    if(passwd_fd < 0)
    {
        perror("open or create /etc/chat.passwd failed");
        exit(1);
    }

    memset(bufname, 0, sizeof(bufname));
    memset(bufpasswd, 0, sizeof(bufpasswd));
    memset(buf, 0, sizeof(buf));
    while( Readline(passwd_fd, buf, sizeof(buf)) != 0 )
    {
        int i;
        DEBUG("buf is %s\n", buf);
        for(i=0; i<100; i++)
        {
            if(buf[i] == ':')
            {
                DEBUG("buf[%d] = :\n", i);
                memcpy(bufname, buf, i);
                memcpy(bufpasswd, &buf[i+1], sizeof(buf) - i - 1);
                DEBUG("bufname is :%s\n", bufname);
                DEBUG("info->UserName is :%s\n", info->UserName);
                if( !strncmp(bufname, info->UserName, i) )
                {
                    if(!strncmp(bufpasswd, info->UserPasswd, strlen(bufpasswd) - i - 1))
                    {
                        DEBUG("login success\n");
                        login_flag[fdindex] = TRUE;
                        loginresult = 'Y';
                        n = Writen(sockfd, &loginresult, 1);
                        printf("write %d byte\n", n);
                        close(passwd_fd);
                        return;
                    }
                }
                break;
            }
        }


    }
    if(!login_flag[fdindex])
    {
        loginresult = 'N';
        n = Writen(sockfd, &loginresult, 1);
        printf("write %d byte\n", n);
        DEBUG("login fail\n");
    }
    DEBUG("exit handle_login\n");
    close(passwd_fd);
}


void str_echo(int listenfd)
{
    int maxi, maxfd, nready, i, n, connfd;
    int cliselfd[FD_SETSIZE];
    fd_set rdset;
    int login_ok[FD_SETSIZE];
    struct sockaddr_in cliaddr;
    struct chat_info cli_info;
    time_t ticks;
    memset(&cli_info, 0, sizeof(struct chat_info));
    for(i=0; i<FD_SETSIZE; i++)
    {
        cliselfd[i] = -1;
        login_ok[i] = FALSE;
    }
    cliselfd[0] = listenfd;
    maxfd = listenfd;
    maxi = 0;
    FD_ZERO(&rdset);
    for(;;)
    {
        for(i=0; i<=maxi; i++)
        {
            if( (cliselfd[i] != -1) )
            {
                FD_SET(cliselfd[i], &rdset);
            }
        }
        nready = Select(maxfd+1, &rdset, NULL, NULL, NULL);
        for(i=0; i <= maxi; i++)
        {
            if(nready == 0)
            {
                break;
            }

            if(FD_ISSET(cliselfd[0], &rdset))
            {
                socklen_t len;
                len = sizeof(cliaddr);
                connfd = Accept(listenfd, (SA *)&cliaddr, &len);
                printf("%s:%d connected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                nready--;
                if(connfd > maxfd)
                    maxfd = connfd;
                for(i=0; i<FD_SETSIZE; i++)
                {
                    if(cliselfd[i] == -1)
                    {
                        if(i>maxi)
                            maxi=i;
                        cliselfd[i] = connfd;
                        break;
                    }
                }

            }
            if(FD_ISSET(cliselfd[i], &rdset))
            {
                nready--;
                if( (n = Read(cliselfd[i], &cli_info, sizeof(struct chat_info))) <= 0 )
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
                        FD_CLR(cliselfd[i], &rdset);
                        close(cliselfd[i]);
                        cliselfd[i] = -1;
                        continue;
                    }
                }

                if(cli_info.flag == REGISTER)
                {
                    printf("file:%s,line:%d\n", __FILE__, __LINE__);
                    reg_to_passwd_file(&cli_info, "/etc/chat.passwd");
                }
                else if(cli_info.flag == LOGIN)
                {
                    printf("file:%s,line:%d\n", __FILE__, __LINE__);
                    printf("login cli_info.name is %s\n", cli_info.UserName);
                    handle_login(&cli_info, "/etc/chat.passwd", login_ok, i, cliselfd[i]);
                }
                else if(cli_info.flag == SENDMSG)
                {
                    printf("file:%s,line:%d\n", __FILE__, __LINE__);
                    for(i=1; i<=maxi; i++)
                    {
                        if(cliselfd[i] != -1)
                        {
                            if(!login_ok[i])
                                continue;
                            ticks = time(NULL);
                            snprintf(cli_info.RealTime, sizeof(cli_info.RealTime), "%.24s", ctime(&ticks));
                            //memcpy(cli_info.RealTime, ctime(time(NULL)), );
                            Writen(cliselfd[i], &cli_info, sizeof(struct chat_info) - (MAXLINE-strlen(cli_info.msg)));
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
        DEBUG("wait for data\n");
        Select(maxfd, &sel_rdset, NULL, NULL, NULL);
        DEBUG("select return\n");
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
            DEBUG("sockfd in %s %d has data\n", __FILE__, __LINE__);
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
            DEBUG("stdin in %s has data\n", __FILE__);
            Writen(fd, msginfo, sizeof(struct chat_info) - (MAXLINE-strlen(buf)));
            memset(buf, 0, MAXLINE);
            memset(msginfo->msg, 0, MAXLINE);
        }

    }

}

int file_exists(char *filename)
{
return (access(filename, 0) == 0);
}

