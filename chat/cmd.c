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
    for(i=0; i<=maxi; i++)
    {
        if(login_ok)
            Writen(fd, info->cliname, sizeof(info->cliname));
    }
}

void srv_handle_cmd(int fd, struct chat_info *info, int *login_ok, int maxi)
{
    DEBUG("cmd is:%s\n", info->cmd);
    char unsupport[] = "unsupport instuction";
    if( !strncmp(info->cmd, "onlinenum", 9) )
    {
        write_online_num_to_cli(fd, login_ok, maxi);
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
    char buf[30];
    memset(buf, 0, sizeof(buf));

    if( !strncmp(msginfo->cmd, "onlinenum", 9) )
    {
        Read(fd, buf, sizeof(buf));
        printf(YELLOW"online people:%s\n"COLOR_NONE, buf);
    }
    else
    {
        Read(fd, buf, sizeof(buf));
        printf(LIGHT_RED"%s\n"COLOR_NONE, buf);
    }
}
