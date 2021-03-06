#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define PORT 123412
#define IPADDR "127.0.0.1"

int main(int argc, char *argv[])
{
	int client_fd, len, msg_size;
	struct sockaddr_in client_addr;
	
	char buf[BUF_SIZE];
	char recv_buf[BUF_SIZE];

	client_fd = socket(PF_INET, SOCK_STREAM, 0);

	client_addr.sin_addr.s_addr = inet_addr(IPADDR);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);

	if(connect(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
	{
		printf("Can't connect\n");
		close(client_fd);
		return -1;
	}

	while(1){
		printf("send : ");
		gets(buf);
		if(strcmp(buf, "exit") == 0)
			break;
		if(send(client_fd, buf, 0xff, 0) == -1){
			perror("send : ");
			break;
		}
		if(msg_size = recv(client_fd, (char *)recv_buf, BUF_SIZE, 0) == 0){
			perror("recv ");
			break;
		}
		printf("echo : %s\n", recv_buf);
	}
	close(client_fd);

	return 0;
}
