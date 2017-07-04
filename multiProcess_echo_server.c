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
	int listen_fd, connect_fd;
	int len, msg_size;
	int pid;
	char test[20];

	if(argc != 2)
	{
		printf("Usage ./filename [PORT] \n");
		exit(0);
	}
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Server : can not Open Socket\n");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		printf("Server : can not bind local address\n");
		exit(0);
	}
	if(listen(listen_fd, 5) < 0)
	{
		printf("Server : can not listen connect. \n");
		exit(0);
	}
	memset(buffer, 0x00, sizeof(buffer));
	len = sizeof(client_addr);

	printf("========[PORT] : %d ========\n", atoi(argv[1]));
	printf("Server : waiting connection request.\n");

	while(1)
	{
		connect_fd = accept(listen_fd, (struct socketaddr *)&client_addr, (socklen_t *)&len);
		if(connect_fd < 0)
		{
			printf("Server : accept failed\n");
			exit(0);
		}

		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("Server : %s client connect. \n", temp);

		pid = fork();

		if(pid == 0)//when process is child
		{
			printf("enter recv socket\n");
			close(listen_fd);
			while(1){
				if((msg_size = recv(connect_fd, (char *)buffer, BUF_SIZE, 0)) == 0){
					close(connect_fd);	
					printf("Server : %s client closed.\n", temp);
					break;
				}
				else{
					printf("client %s : %s\n",temp, buffer);
					send(connect_fd, buffer, 0xff, 0);
				}
			}
			return 0;
		}
		else if(pid > 0) //when process is parent
		{
			close(connect_fd);
		}
		else if(pid == -1)
			perror("fork error ");
	}
	close(listen_fd);
	return 0;
}
