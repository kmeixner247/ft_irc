#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>
Server::Server()
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

void Server::disconnectClient(int socket)
{
	std::cout << "client disconnected" << std::endl;
	FD_CLR(socket, &this->_readfds);

	this->_registeredclients.erase(this->_connectedclients[socket].getNickname());
	if (!this->_connectedclients.erase(socket))
		throw "invalid socket disconnect";
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
							this->disconnectClient(it->first);
							break ;	//segfaulted with a heap-use-after-free on line 68 without this. I assume it's because the iterator position is skewed after deleting an element. would be nicer to not have to break here
						}
						else
							this->interpretMessages(buffer);
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
		msgs.push_back(Message(temp.substr(0, temp.find('\n') - 1)));
		temp = temp.substr(temp.find('\n') + 1, temp.npos);
	}
	return (msgs);
}

void Server::interpretMessages(char *buffer)
{
	std::vector<Message> msgs = parseMessages(buffer);
	for (std::vector<Message>::iterator it = msgs.begin(); it != msgs.end(); it++)
	{
		std::string command = it->getCommand();

		if (!(command.compare("PASS"))) this->PASS(*it);
		else if (!command.compare("USER")) this->USER(*it);
		else if (!command.compare("NICK")) this->NICK(*it);
		else std::cout << "NONE OF THOSE" << std::endl;
	}
}

// commands
void Server::PASS(Message msg)
{
	std::cout << "PASS" << std::endl;
	std::cout << msg << std::endl;
}
void Server::NICK(Message msg)
{
	// if there is a prefix; change user with nick prefix to parameter
	// if no prefix, introducing new nick for user
	std::cout << "NICK" << std::endl;
	std::cout << msg << std::endl;
}
void Server::USER(Message msg)
{
	//
	std::cout << "USER" << std::endl;
	std::cout << msg << std::endl;
}
