#ifndef _CMD_H_
#define _CMD_H_
#include "chat.h"
#include "zwunp.h"

void write_online_num_to_cli(int fd, int *login_ok, int maxi);

void write_online_name_to_cli(int *login_ok, struct user_info *info, int maxi, int fd);

void srv_handle_cmd(int fd, struct chat_info *info, int *login_ok, int maxi, struct user_info *uinfo);

void send_cmd_to_srv(int fd, struct chat_info *msginfo);

void recieve_cmd_result_from_srv(int fd, struct chat_info *msginfo);

#endif
