#include "cmd.h"

void write_online_num_to_cli(int fd, int *login_ok, int maxi)
{
    int i;
    char buf[10];
    int num_ret;
    num_ret = 0;
    for(i=0; i<=maxi; i++)
    {
        if(login_ok)
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
    for(i=0; i<=maxi; i++)
    {
        if(login_ok)
        {
            printf("i:%d\n", i);
            printf("info->name:%s\n", info[i].cliname);
            printf("name size is %ld\n", strlen(info[i].cliname));
            n = strlen(info[i].cliname)+1;
            snprintf(&buf[index], n, "%s", info[i].cliname);
            buf[index+n-1] = '\n';
            index += n;
        }
    }

    Writen(fd, buf, strlen(buf));
}

void srv_handle_cmd(int fd, struct chat_info *info, int *login_ok, int maxi, struct user_info *uinfo)
{
    DEBUG("cmd is:%s\n", info->cmd);
    char unsupport[] = "unsupport instuction";
    printf("excute srv_handle_cmd\n");
    if( !strncmp(info->cmd, "onlinenum", 9) )
    {
        write_online_num_to_cli(fd, login_ok, maxi);
    }
    else if( !strncmp(info->cmd, "onlinename", 10) )
    {
        write_online_name_to_cli(login_ok, uinfo, maxi, fd);
    }
    else
        Writen(fd, unsupport, sizeof(unsupport));

}


void send_cmd_to_srv(int fd, struct chat_info *msginfo)
{
    printf("excute send_cmd_to_srv\n");
    Writen(fd, msginfo, sizeof(struct chat_info));
}

void recieve_cmd_result_from_srv(int fd, struct chat_info *msginfo)
{
    char buf[FD_SETSIZE*25];
    //int n;
    printf("excute recieve_cmd_result_from_srv\n");
    memset(buf, 0, sizeof(buf));
    if( !strncmp(msginfo->cmd, "onlinenum", 9) )
    {
        Read(fd, buf, sizeof(buf));
        printf(YELLOW"%s online people\n"COLOR_NONE, buf);
    }
    if( !strncmp(msginfo->cmd, "onlinename", 10) )
    {
        Read(fd, buf, sizeof(buf));
        printf(YELLOW"online people:\n%s\n"COLOR_NONE, buf);
    }
    else
    {
        Read(fd, buf, sizeof(buf));
        printf(LIGHT_RED"%s\n"COLOR_NONE, buf);
    }
}
