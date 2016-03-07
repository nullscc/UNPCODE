#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <netinet/in.h>

int main()
{
	int sockfd;
	int Connetedfd;
	char buff[100];
	struct sockaddr_in servaddr; //在netinet/in.h中定义
	time_t ticks;
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		printf("create socket error\n");	
		return -1;
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	inet_aton("127.0.0.1", &servaddr.sin_addr);
	
	if( bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		printf("bind error\n");	
		return -1;
	}
	
	if( listen(sockfd, 10) == -1)
	{
		printf("listen failed\n");
		return -1;	
	}
	
	for( ; ;)
	{
		Connetedfd = accept(sockfd, NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(Connetedfd, &buff, strlen(buff));
		close(Connetedfd);
		break;
	}
	
	return 0;
}