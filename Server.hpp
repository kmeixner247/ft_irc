#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "Client.hpp"
#include "Message.hpp"

class Server
{
private:
	int _serverfd;
	struct sockaddr_in _address;
	std::map<int, Client> _clients;
	fd_set readfds;

public:
	Server();
	~Server();
	void serverloop();
	void testloop();
	void connectClient(int socket);
	void sendMsg(Client client, Message msg) const;
	void sendMsg(Client client, std::string msg) const;
	void sendMsg(Client client, char *msg) const;
private:
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
	std::vector<Message> parseMessages(char *input);
};

#endif