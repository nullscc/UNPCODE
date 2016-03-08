#include <stdio.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <strings.h>
 
int main(int argc, char **argv)
{
		int sock_fd, n;
		int ret;
		char recieve[100];
		struct sockaddr_in serv_addr; //serv_addr
		if( argc != 2)
		{
			printf("Usage:%s <IPaddress>\n", argv[0]);
			return -1;
		}
		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(13);
		//inet_aton("127.0.0.1", &serv_addr.sin_addr);
		inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
		
		if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("create socket failed");
			return -1;	
		}
	
		if((ret = connect(sock_fd, (const struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in))) == -1)
		{
			perror("connect error");	
			return -1;
		}
		
		while((n = read(sock_fd, recieve, 100)) > 0)
		{
			recieve[n] = 0;
			if(fputs(recieve, stdout) == EOF)	
			{
				perror("error fputs");	
			}
		}
		
		if(n<0)
		{
				perror("read error");
		}
		return 0;
}
