#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <map>
#include <set>
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
	std::map<std::string, Client> _registeredclients;
	fd_set _readfds;
	std::map<int, Client> _connectedclients;

public:
	Server();
	~Server();
	void serverloop();
	void connectClient(int socket);
	void disconnectClient(int socket);
	void interpretMessages(char *buffer);
	void sendMsg(Client client, Message msg) const;
	void sendMsg(Client client, std::string msg) const;
	void sendMsg(Client client, char *msg) const;
private:
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
	std::vector<Message> parseMessages(char *input);
	// commands
	void PASS(Message msg);
	void USER(Message msg);
	void NICK(Message msg);
};

#endif
