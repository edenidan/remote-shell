#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr


void strcatPrefix(char* s,char* prefix)
{
	int sLen = strlen(s);
	int prefixLen = strlen(prefix);

	int oldEndIndex = strlen(s)-1;
	
	int i = 0;
	for(i = 0; i < sLen ; i++)
		s[oldEndIndex - i + prefixLen] = s[oldEndIndex - i];
	
	i = 0;
	while(prefix[i] != '\0'){
		s[i] = prefix[i];
		i++;
	}
}
void func(int sockfd)
{
	char prefixToSend[MAX] = "echo p ;";
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("$");
		n = 0;

		while ((buff[n++] = getchar()) != '\n');
		strcatPrefix(buff,prefixToSend);

		write(sockfd, buff, strlen(buff));

		char expectedExit[MAX] = "exit\n";
		strcatPrefix(expectedExit,prefixToSend);
		if ((strcmp(buff, expectedExit)) == 0) {
			printf("Client Exit...\n");
			break;
		}

		bzero(buff, sizeof(buff));
		
		usleep(250000);
		read(sockfd, buff, sizeof(buff));
		printf("%s", buff + 2);
		
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}

