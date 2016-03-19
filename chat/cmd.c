#include "cmd.h"

void write_online_num_to_cli(int fd, int *login_ok, int maxi)
{
    int i;
    char buf[10];
    int num_ret;
    num_ret = 0;
    for(i=1; i<=maxi; i++)
    {
        if(login_ok[i])
            num_ret++;
    }
    snprintf(buf, 10, "%d", num_ret);
    Writen(fd, buf, strlen(buf));
}

void write_online_name_to_cli(int *login_ok, struct user_info *info, int maxi, int fd)
{
    int i;
    char buf[FD_SETSIZE*25];
    int index = 0;
    int n;
    memset(buf, 0, sizeof(buf));//如果不加这句，会有有趣的事情出现
    for(i=1; i<=maxi; i++)
    {
        if(login_ok[i])
        {
            n = strlen(info[i].cliname)+1;
            snprintf(&buf[index], n, "%s", info[i].cliname);
            buf[index+n-1] = '\n';
            index += n;
        }
    }

    Writen(fd, buf, strlen(buf));
}

void write_help_info_to_cli(int fd)
{
    char buf[FD_SETSIZE*25];
    int helpfd;
    memset(buf, 0, sizeof(buf));
    helpfd = open("/etc/chat/help", O_RDONLY);
    Read(helpfd, buf, FD_SETSIZE*25);
    Writen(fd, buf, strlen(buf));
}

void srv_handle_cmd(int fd, struct chat_info *info, int *login_ok, int maxi, struct user_info *uinfo)
{
    DEBUG("cmd is:%s\n", info->cmd);
    char unsupport[] = "unsupport instuction";
    DEBUG("excute srv_handle_cmd\n");
    if( !strncmp(info->cmd, "onlinenum", 9) )
    {
        write_online_num_to_cli(fd, login_ok, maxi);
    }
    else if( !strncmp(info->cmd, "onlinename", 10) )
    {
        write_online_name_to_cli(login_ok, uinfo, maxi, fd);
    }
    else if( !strncmp(info->cmd, "help", 4) )
    {
        write_help_info_to_cli(fd);
    }
    else
        Writen(fd, unsupport, sizeof(unsupport));

}


void send_cmd_to_srv(int fd, struct chat_info *msginfo)
{
    Writen(fd, msginfo, sizeof(struct chat_info));
}

void recieve_cmd_result_from_srv(int fd, struct chat_info *msginfo)
{
    char buf[FD_SETSIZE*25];
    //int n;
    DEBUG("excute recieve_cmd_result_from_srv\n");
    memset(buf, 0, sizeof(buf));
    if( !strncmp(msginfo->cmd, "onlinenum", 9) )
    {
        Read(fd, buf, sizeof(buf));
        printf(YELLOW"%s online people\n"COLOR_NONE, buf);
    }
    else if( !strncmp(msginfo->cmd, "onlinename", 10) )
    {
        Read(fd, buf, sizeof(buf));
        printf(YELLOW"online people:\n%s\n"COLOR_NONE, buf);
    }
    else if( !strncmp(msginfo->cmd, "help", 4) )
    {
        Read(fd, buf, sizeof(buf));
        printf("-------------------------------------------------------------------\n");
        printf(LIGHT_CYAN"\nBelow Is Help Info.\n\n"COLOR_NONE);
        printf(YELLOW"%s\n"COLOR_NONE, buf);
        printf("-------------------------------------------------------------------\n");
    }
    else
    {
        Read(fd, buf, sizeof(buf));
        printf(LIGHT_RED"%s\n"COLOR_NONE, buf);
    }
}

void get_prvname(char *prvname,char *buf)
{
    memset(prvname, 0, 25);
    int i;
    for(i=0; i<25; i++)
    {
        if(buf[i] == ' ')
            break;
    }
    memcpy(prvname, buf, i);
}

void get_prvmsg(char *prvmsg, char *buf)
{
    memset(prvmsg, 0, MAXLINE);
    int i;
    for(i=0; i<MAXLINE; i++)
    {
        if(buf[i] == ' ')
            break;
    }
    memcpy(prvmsg, &buf[i+1], strlen(&buf[i+1]));
}

void srv_handle_prv_chat(int sendinex, int *clifd, struct chat_info *info, int *login_ok, int maxi, struct user_info *uinfo)
{
    int i;
    char errmsg[] = "Your @user is not online or not exist!!!";
    for(i=1;i<=maxi;i++)
    {
        if(login_ok[i])
        {
            if( !strncmp(info->PrvName, uinfo[i].cliname, strlen(info->PrvName)) )
            {
                time_t ticks;
                ticks = time(NULL);
                snprintf(info->RealTime, sizeof(info->RealTime), "%.24s", ctime(&ticks));
                Writen(clifd[i], info, sizeof(struct chat_info) - (MAXLINE-strlen(info->msg)));
                memcpy(info->msg, errmsg, sizeof(errmsg));
                return;
            }
        }
    }

    memcpy(info->msg, errmsg, sizeof(errmsg));
    Writen(clifd[sendinex], info, sizeof(struct chat_info) - (MAXLINE-strlen(info->msg)));
}
