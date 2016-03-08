#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <netinet/in.h>

int main()
{
	int listenfd;
	int Connfd; 
	char buff[100];
	struct sockaddr_in servaddr;
	time_t ticks;
	if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("create socket error");	
		return -1;
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	//inet_aton("127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind error");	
		return -1;
	}
	
	if( listen(listenfd, 10) == -1)
	{
		perror("listen failed");
		return -1;	
	}
	
	for( ; ;)
	{
		Connfd = accept(listenfd, NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(Connfd, &buff, strlen(buff));
		close(Connfd);
	}
	
	return 0;
}