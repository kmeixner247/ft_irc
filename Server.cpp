#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>


Server::Server(int port, std::string pw) : _port(port), _password(pw), _host("127.0.0.1"), _servername("awesomeserverofawesomeness"), _version("69.69"), _motd("kacper smells")
{
	if (this->init())
		throw "something went wrong in init";
}

Server::~Server()
{
	shutdown(this->_serverfd, SHUT_RDWR);
}

void Server::connectClient(int socket)
{
	FD_SET(socket, &this->_readfds);
	Client newclient(socket);
	this->_connectedclients[socket] = newclient;
	std::cout << "client connected" << std::endl;
}

void Server::disconnectClient(Client *cl)
{
	std::cout << "client disconnected" << std::endl;
	FD_CLR(cl->getSocket(), &this->_readfds);

	this->_registeredclients.erase(cl->getNickname());
	if (!this->_connectedclients.erase(cl->getSocket()))
		throw "invalid socket disconnect";
}

void Server::serverloop()
{
	int addrlen = sizeof(this->_address);
	int newfd;
	int highest_socket;
	char buffer[1024] = {0};
	fd_set readfds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	std::string temp;
	while (42)
	{
		// checking whether there's a new client trying to connect
		// (make this a loop in case several try to connect at the same time?)
		if ((newfd = accept(this->_serverfd, (struct sockaddr *)&this->_address, (socklen_t *)&(addrlen))) > 0)
		{
			// making the client fd nonblocking (is this necessary?)
			if (fcntl(newfd, F_SETFL, O_NONBLOCK) == -1)
			{
				perror("fcntl failed");
				exit(EXIT_FAILURE);
			} 
			this->connectClient(newfd);
		}
		// get highest socket for select. is there a better way to do this?#
		if (this->_connectedclients.size() > 0)
			highest_socket = this->_connectedclients.rbegin()->first;
		readfds = this->_readfds;
		if (highest_socket > 0)
		{
			if (select(highest_socket + 1, &readfds, NULL, NULL, &tv) > 0)
			{
				iterator tempend = this->_connectedclients.end();
				for (iterator it = this->_connectedclients.begin(); it != tempend; it++)
				{
					if (FD_ISSET(it->first, &readfds))
					{
						if (!recv(it->first, buffer, 1024, 0))
						{
							//pls prettify
							this->disconnectClient(&it->second);
							if (_connectedclients.size() == 0)
							{
								memset(buffer, 0, 1024);
								break ;
							}
							tempend = this->_connectedclients.end();
							it = this->_connectedclients.begin();
						}
						else
							this->interpretMessages(&it->second, buffer);
						memset(buffer, 0, 1024);
					}
				}
			}
		}
	}
}

int Server::init()
{
	int opt = 1;
	if ((this->_serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("server init: socket failed");
		return (-1);
	}
	if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("server init: setsockopt failed");
		return (-1);
	}
	if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("server init: setsockopt failed");
		return (-1);
	}
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
	if (bind(this->_serverfd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		perror("server init: bind failed");
		return (-1);
	}
	if (listen(this->_serverfd, 3) < 0)
	{
		perror("server init: listen failed");
		return (-1);
	}
	if (fcntl(this->_serverfd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl failed");
		return (-1);
	}
	return (0);
}

void Server::sendMsg(Client *cl, Message msg) const
{
	std::string temp = msg.getRaw();
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
}

void Server::sendMsg(Client *cl, std::string msg) const
{
	send(cl->getSocket(), msg.c_str(), msg.length(), 0);
}

void Server::sendMsg(Client *cl, char *msg) const
{
	send(cl->getSocket(), msg, std::string(msg).length(), 0);
}


std::vector<Message> Server::parseMessages(char *input)
{
	std::vector<Message> msgs;
	std::string temp(input);
	size_t pos;
	while ((pos = temp.find('\n')) != temp.npos)
	{
		msgs.push_back(Message(temp.substr(0, pos - 1)));
		temp = temp.substr(pos + 1, temp.npos);
	}
	return (msgs);
}

void Server::interpretMessages(Client *cl, char *buffer)
{
	std::vector<Message> msgs = parseMessages(buffer);
	for (std::vector<Message>::iterator it = msgs.begin(); it != msgs.end(); it++)
	{
		//errors?
		std::string command = it->getCommand();
		if (!(command.compare("PASS")))
			this->PASS(cl, *it);
		else// if (this->clientIsRegistered(cl))
		{
			if (!cl->getPassbool())
				return ;
			if (!command.compare("USER")) this->USER(cl, *it);
			else if (!command.compare("NICK")) this->NICK(cl, *it);
			else if (!(command.compare("QUIT"))) this->QUIT(cl, *it);
			else if (!(command.compare("KILL"))) this->KILL(cl, *it);
			else if (!(command.compare("OPER"))) this->OPER(cl, *it);
			else if (!(command.compare("SQUIT"))) this->SQUIT(cl, *it);
			else if (!(command.compare("PRIVMSG"))) this->PRIVMSG(cl, *it);
			else if (!(command.compare("WALLOPS"))) this->WALLOPS(cl, *it);
			else if (!(command.compare("NOTICE"))) this->NOTICE(cl, *it);
			else if (!(command.compare("KICK"))) this->KICK(cl, *it);
			else if (!(command.compare("MODE"))) this->MODE(cl, *it);
			else if (!(command.compare("INVITE"))) this->INVITE(cl, *it);
			else if (!(command.compare("TOPIC"))) this->TOPIC(cl, *it);
			else std::cout << "NONE OF THOSE1\n" << *it << std::endl;
		}
/* 		else if(!(this->clientIsRegistered(cl)))
		{
			if (!cl->getPassbool())
				return ;
			if (!command.compare("USER")) this->USER(cl, *it);
			else if (!command.compare("NICK")) this->NICK(cl, *it);
			else std::cout << "NONE OF THOSE2\n" << *it << std::endl;
		} */
	}
}

void Server::sendResponse(Client *cl, std::string msg)
{
	this->sendMsg(cl, replace_thingies(msg, cl));
}

void Server::sendWelcome(Client *cl)
{
	//this is kinda temporary
	this->sendResponse(cl, RPL_WELCOME);
	this->sendResponse(cl, RPL_YOURHOST);
	this->sendResponse(cl, RPL_CREATED);
	this->sendResponse(cl, RPL_MYINFO);
	this->sendResponse(cl, RPL_MOTDSTART);
	this->sendResponse(cl, RPL_MOTD);
	this->sendResponse(cl, RPL_ENDOFMOTD);
}

std::string Server::replace_thingies(std::string msg, Client *cl)
{
	int pos;
	while ((pos = msg.find("<nick>") != std::string::npos))
		msg.replace(msg.find("<nick>"), 6, cl->getNickname());
	while ((pos = msg.find("<user>") != std::string::npos))
		msg.replace(msg.find("<user>"), 6, cl->getUsername());
	while ((pos = msg.find("<host>") != std::string::npos))
		msg.replace(msg.find("<host>"), 6, this->_host);
	while ((pos = msg.find("<version>") != std::string::npos))
		msg.replace(msg.find("<version>"), 9, this->_version);
	while ((pos = msg.find("<server>") != std::string::npos))
		msg.replace(msg.find("<server>"), 8, this->_servername);
	//probably more
	return (msg);
}
std::string Server::replace_thingies(std::string msg, Client *cl, Channel *ch)
{
	//int pos;
	(void)ch;
	msg = this->replace_thingies(msg, cl);

	//probably more
	return (msg);
}

bool Server::clientIsConnected(Client *cl)
{
	return !(this->_connectedclients.find(cl->getSocket()) == _connectedclients.end());
}

bool Server::clientIsRegistered(Client *cl)
{
	return !(this->_registeredclients.find(cl->getNickname()) == _registeredclients.end());
}