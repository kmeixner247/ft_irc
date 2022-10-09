#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

class Server
{
private:
	int _serverfd;
	struct sockaddr_in _address;
	std::vector<int> _clients;

public:
	Server();
	~Server();
	void serverloop();
private:
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
};

#endif