/*server.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	char buffer[BUF_SIZE];
	struct sockaddr_in server_addr, client_addr;
	char temp[20];
	int server_fd, client_fd;
	int len, msg_size;
	char test[20];
	if(argc != 2)
	{
		printf("Usage ./filename [PORT] \n");
		exit(0);
	}
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Server : can not Open Socket\n");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		printf("Server : can not bind local address\n");
		exit(0);
	}
	
	if(listen(server_fd, 5) < 0)
	{
		printf("Server : can not listen connect. \n");
		exit(0);
	}

	memset(buffer, 0x00, sizeof(buffer));
	len = sizeof(client_addr);
	printf("========[PORT] : %d ========\n", atoi(argv[1]));
	printf("Server : waiting connection request.\n");

	while(1){
		server_fd = accept(server_fd, (struct socketaddr *)&client_addr, (socklen_t *)&len);
		if(server_fd <0)//client_fd < 0)
		{
			printf("Server : accept failed\n");
			exit(0);
		}
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("Server : %s client connect. \n", temp);

		while(1){
			if((msg_size = recv(server_fd/*client*/, (char *)buffer, BUF_SIZE, 0)) == 0){
				close(server_fd);	
				printf("Server : %s client closed.\n", temp);
				break;
			}
			printf("client %s : %s\n",temp, buffer);
			send(server_fd, buffer, 0xff, 0);
		}
		//close(server_fd);
		return 0;
	}
}
