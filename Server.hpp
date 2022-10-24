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
#include "Channel.hpp"

class Server
{
	typedef std::map<int, Client>::iterator	iterator;
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
	std::string _passwordOper;
	std::map<std::string, Channel*> _channels;

public:
	Server(int port, std::string pw);
	~Server();
	void serverloop();
	void connectClient(int socket);
	void disconnectClient(Client *cl);
	// void interpretMessages(Client *cl, char *buffer);
	void receiveMessage(Client *cl, char *buffer);
	void interpretMessages(Client *cl, std::vector<Message> msgs);
	void sendMsg(Client *cl, Message msg) const;
	void sendMsg(Client *cl, std::string msg) const;
	void sendMsg(Client *cl, char *msg) const;
	void setPort(int port);
	void setPassword(std::string pw);
	void setRegisteredClients(std::map<std::string, Client*> clients);
	std::map<std::string, Client*> getRegisteredClients();
	void setConnectedClients(std::map<int, Client> clients);
	std::map<int, Client> getConnectedClients();
	void setHost(std::string host);
	std::string getHost();
	void setServerName(std::string name);
	std::string getServerName();
	void setVersion(std::string version);
	std::string getVersion();
	void setMotd(std::string motd);
	std::string getMotd();
	void setPasswordOper(std::string pw);
	std::string getPasswordOper();
	void setChannels(std::map<std::string, Channel*> channels);
	std::map<std::string, Channel*> getChannels();
private:
	Server();
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
	// std::vector<Message> parseMessages(char *input);
	std::vector<Message> parseMessages(Client *cl, std::string input);
	std::string getPassword() const;
	int getPort() const;
	// commands
	void PASS(Client *cl, Message msg);
	void USER(Client *cl, Message msg);
	void NICK(Client *cl, Message msg);
	void QUIT(Client *cl, Message msg);
	void KILL(Client *cl, Message msg);
	void OPER(Client *cl, Message msg);
	void SQUIT(Client *cl, Message msg);
	void PRIVMSG(Client *cl, Message msg);
	void WALLOPS(Client *cl, Message msg);
	void NOTICE(Client *cl, Message msg);
	void KICK(Client *cl, Message msg);
	void MODE(Client *cl, Message msg);
	void INVITE(Client *cl, Message msg);
	void TOPIC(Client *cl, Message msg);
	void WHO(Client *cl, Message msg);

	void sendWelcome(Client *cl);
	std::string replace_thingies(std::string msg, Client *cl);
	std::string replace_thingies(std::string msg, Client *cl, Channel *ch);
	void sendResponse(Client *cl, std::string msg);
	bool clientIsConnected(Client *cl);
	bool clientIsRegistered(Client *cl);

};

#endif
