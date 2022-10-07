#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

void init()
{
	
}

int main()
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;
    char buffer[1024] = { 0 };
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	// if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	// {
	// 	perror("setsockopt");
	// 	exit(EXIT_FAILURE);
	// }
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

    if (listen(sockfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
// 	int new_socket;
// 	// if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
// 	// {
// 	// 	perror("fcntl failed");
// 	// 	exit (EXIT_FAILURE);
// 	// }
// 	if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
// 	{
// 		perror("accept");
// 		exit(EXIT_FAILURE);
// 	}

// 	int valread;
// 	if ((valread = read(new_socket, buffer, 1024)) == -1)
// 		perror ("read failed");
// 	printf("%s", buffer);
// 	close(new_socket);
// 	shutdown(sockfd, SHUT_RDWR);
// }
int main()
{
	while (69)
	{
		if (checkifnewclient)
			printf("hello nu client\n");
	}
}