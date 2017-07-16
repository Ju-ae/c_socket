/*multiThread_server.c*/

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
#include <pthread.h>

#define BUF_SIZE 1024
#define CLIENT_NUM 20

void *recv_thread(void *arg);

pthread_t client[CLIENT_NUM];

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	char temp[20];
	int listen_fd, connect_fd;
	int len, msg_size;
	int pid;
	char test[20];
	int client_turn = 0;
	
	memset(client, 0, sizeof(client));
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

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("Server : can not bind local address\n");
		exit(0);
	}

	if(listen(listen_fd, 5) < 0)
	{
		printf("Server : can not listen connect. \n");
		exit(0);
	}

	//memset(buffer, 0x00, sizeof(buffer));
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
		pthread_create(&client[client_turn], NULL, recv_thread, (void*)&connect_fd);
		
		pthread_detach(client[client_turn]);

		if(client_turn < CLIENT_NUM)
			client_turn += 1;
		else
			client_turn = 0;
	}
	close(listen_fd);
	return 0;
}

void *recv_thread(void *arg)
{
	int connect_fd;
	int recvn, sendn;
	char buffer[BUF_SIZE];
	
	connect_fd = *((int*) arg);
	printf("connect_Fd : %d\n", connect_fd);

	while(1)
	{
		if((recvn = recv(connect_fd, (char *)buffer, BUF_SIZE, 0)) <= 0)
		{
			printf("socket closed.\n");
			close(connect_fd);	
			//		client[client_turn] = -1;
			break;
		}
		else{	
			sendn = send(connect_fd, buffer, 0xff, 0);
			printf("%s\n", buffer);
			if(sendn == -1){
				printf("send error");
				break;
			}
			memset(buffer, 0x00, sizeof(buffer));
		}
	}
}
