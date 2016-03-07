#include <stdio.h>
#include <sys/types.h> 
 #include <netinet/in.h>
 
int main()
{
		int sock_fd, n;
		int ret;
		char recieve[100];
		struct sockaddr_in time_addr;
		time_addr.sin_family = AF_INET;
		time_addr.sin_port = htons(13);
		inet_aton("127.0.0.1", &time_addr.sin_addr);
		
		if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("create socket failed\n");
			return -1;	
		}
	
		if((ret = connect(sock_fd, (const struct sockaddr*)&time_addr, sizeof(struct sockaddr_in))) == -1)
		{
			printf("connect error\n");	
			return -1;
		}
		
		while((n = read(sock_fd, recieve, 100)) > 0)
		{
			recieve[n] = 0;
			if(fputs(recieve, stdout) == EOF)	
			{
				printf("error fputs\n");	
			}
		}
		
		if(n<0)
		{
				printf("read error\n");
		}
		return 0;
}