#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>
Server::Server() : _host("127.0.0.1"), _servername("awesomeserverofawesomeness"), _version("69.69"), _motd("kacper smells")
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
	// std::cout << this->_registeredclients.size() << std::endl;
	// std::cout << this->_connectedclients.size() << std::endl;
}

void Server::serverloop()
{
	int addrlen = sizeof(this->_address);
	int newfd;
	int highest_socket;
	char buffer[1024] = {0};
	fd_set readfds;
	std::string temp;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
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
				for (std::map<int, Client>::iterator it = this->_connectedclients.begin(); it != this->_connectedclients.end(); it++)
				{
					if (FD_ISSET(it->first, &readfds))
					{
						if (!recv(it->first, buffer, 1024, 0))
						{
							this->disconnectClient(&it->second);
							break ;	//segfaulted with a heap-use-after-free on line 68 without this. I assume it's because the iterator position is skewed after deleting an element. would be nicer to not have to break here
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
	this->_address.sin_port = htons(8080); // this'll be the passed port eventually
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

void Server::sendMsg(Client client, Message msg) const
{
	std::string temp = msg.getRaw();
	send(client.getSocket(), temp.c_str(), temp.length(), 0);
}

void Server::sendMsg(Client client, std::string msg) const
{
	send(client.getSocket(), msg.c_str(), msg.length(), 0);
}

void Server::sendMsg(Client client, char *msg) const
{
	send(client.getSocket(), msg, std::string(msg).length(), 0);
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
	(void)cl;
	for (std::vector<Message>::iterator it = msgs.begin(); it != msgs.end(); it++)
	{
		std::string command = it->getCommand();

		if (!(command.compare("PASS"))) this->PASS(cl, *it);
		else if (!command.compare("USER")) this->USER(cl, *it);
		else if (!command.compare("NICK")) this->NICK(cl, *it);
		else std::cout << "NONE OF THOSE\n" << *it << std::endl;
	}
}

// commands
void Server::PASS(Client *cl, Message msg)
{
	(void)cl;
	std::cout << "PASS" << std::endl;
	std::cout << msg << std::endl;
}
void Server::NICK(Client *cl, Message msg)
{
	// if there is a prefix; change user with nick prefix to parameter
	// if no prefix, introducing new nick for user
	std::cout << "NICK" << std::endl;
	std::cout << msg << std::endl;
	if (msg.getPrefix() != "")
		this->_registeredclients.erase(cl->getNickname());
	cl->setNickname(msg.getParameters().back());
	if (cl->getUsername() != "" && cl->getRealname() != "")
	{
		this->_registeredclients[cl->getNickname()] = cl;	//ALSO WELCOME MESSAGE YO
		this->sendWelcome(cl);
	}
	//ERRORS TBD
}
void Server::USER(Client *cl, Message msg)
{
	std::cout << "USER" << std::endl;
	std::cout << msg << std::endl;
	cl->setUsername(msg.getParameters()[0]);
	cl->setRealname(msg.getParameters().back());
	if (cl->getNickname() != "")
	{
		this->_registeredclients[cl->getNickname()] = cl;	//ALSO WELCOME MESSAGE YO
		this->sendWelcome(cl);
	}

}

void Server::sendWelcome(Client *cl)
{
	//this is kinda temporary

	std::string temp(RPL_WELCOME);
	std::cout << this->replace_thingies(RPL_WELCOME, cl)<<std::endl;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_YOURHOST;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_CREATED;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_MYINFO;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_MOTDSTART;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_MOTD;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
	temp = RPL_ENDOFMOTD;
	send(cl->getSocket(), temp.c_str(), temp.length(), 0);
}

std::string Server::replace_thingies(std::string msg, Client *c)
{
	int pos;
	if ((pos = msg.find("<nick>") != std::string::npos))
		msg.replace(msg.find("<nick>"), 6, c->getNickname());
	if ((pos = msg.find("<user>") != std::string::npos))
		msg.replace(msg.find("<user>"), 6, c->getUsername());
	if ((pos = msg.find("<host>") != std::string::npos))
		msg.replace(msg.find("<host>"), 6, this->_host);
	if ((pos = msg.find("<version>") != std::string::npos))
		msg.replace(msg.find("<version>"), 9, this->_version);
	if ((pos = msg.find("<server>") != std::string::npos))
		msg.replace(msg.find("<server>"), 9, this->_servername);
	return (msg);

}