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
#include "Responses.hpp"

class Server
{
private:
	int _port;
	std::string _password;
	int _serverfd;
	struct sockaddr_in _address;
	std::map<std::string, Client*> _registeredclients;
	fd_set _readfds;
	std::map<int, Client> _connectedclients;
	std::string _host;
	std::string _servername;
	std::string _version;
	std::string _motd;

public:
	Server(int port, std::string pw);
	~Server();
	void serverloop();
	void connectClient(int socket);
	void disconnectClient(Client *cl);
	void interpretMessages(Client *cl, char *buffer);
	void sendMsg(Client *cl, Message msg) const;
	void sendMsg(Client *cl, std::string msg) const;
	void sendMsg(Client *cl, char *msg) const;
private:
	Server();
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
	std::vector<Message> parseMessages(char *input);
	std::string getPassword() const;
	int getPort() const;
	// commands
	void PASS(Client *cl, Message msg);
	void USER(Client *cl, Message msg);
	void NICK(Client *cl, Message msg);
	void sendWelcome(Client *cl);
	std::string replace_thingies(std::string msg, Client *cl);
	void sendResponse(Client *cl, std::string msg);

};

#endif
