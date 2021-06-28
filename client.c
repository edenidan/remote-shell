#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080

void strcatPrefix(char* s,char* prefix) {
	int sLen = strlen(s);
	int prefixLen = strlen(prefix);

	int oldEndIndex = strlen(s) - 1;
	
	int i = 0;
	for(i = 0; i < sLen; i++)
		s[oldEndIndex - i + prefixLen] = s[oldEndIndex - i];
	
	i = 0;
	while(prefix[i] != '\0'){
		s[i] = prefix[i];
		i++;
	}
}

void func(int sockfd) {
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
		strcatPrefix(expectedExit, prefixToSend);
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

int main() {
	int sockfd, connfd;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("error in socket()\n");
		exit(0);
	}
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		printf("error in connect()\n");
		exit(0);
	}
	printf("connected to the server\n");

	func(sockfd);

	close(sockfd);
}

