#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
Server::Server()
{
	if (this->init())
		throw "temp";
}

Server::~Server()
{
	shutdown(this->_serverfd, SHUT_RDWR);
}

void analyze_string(char *str)
{
	int i = 0;
	while (str[i])
	{
		std::cout << i << ": " << static_cast<int>(str[i]) << " " << str[i] <<  std::endl;
		i++;
	}
}

void Server::connectClient(int socket)
{
	Client newclient(socket);
	std::vector<Message> msgs;
	this->_clients[this->_clients.size()] = newclient;
	char buffer[1024] = {0};
	recv(socket, buffer, 1024, 0);
	std::cout << "FD " << socket << " connected." << std::endl;
	msgs = parseMessages(buffer);
	if (!msgs.size())
		return ;
	//confirm password?
	size_t i = 0;
	while (i < msgs.size() && msgs[i].getCommand().compare("NICK")) { i++; }
	if (i < msgs.size())
		newclient.setNickname(msgs[i].getParameters()[0]);
	i = 0;
	while (i < msgs.size() && msgs[i].getCommand().compare("USER")) { i++; }
	if (i < msgs.size())
		newclient.setUsername(msgs[i].getParameters()[0]);
	std::cout << "MSG SIZE IS " << msgs.size() << std::endl;
	std::string temp = msgs[i].getParameters()[3];
	size_t j = 4;
	while (j <= msgs[i].getParameters().size())
	{
		temp += " ";
		temp += msgs[i].getParameters()[j];
		j++;
	}
	newclient.setRealname(temp);
	this->_clients[this->_clients.size()] = newclient;

	//what to respond to the client?
}

void Server::serverloop()
{
	int addrlen = sizeof(this->_address);
	int newfd;
	int highest_socket;
	char buffer[1024] = { 0 };
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
				exit (EXIT_FAILURE);
			}
			this->connectClient(newfd);
		}
		// clearing the readfds set (necessary?)
		FD_ZERO(&readfds);
		highest_socket = -1;
		// filling the readfds set (for select) while finding the highest socket fd (also for select)
		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			if (this->_clients[i].getSocket() > highest_socket)
				highest_socket = this->_clients[i].getSocket();
			FD_SET(this->_clients[i].getSocket(), &readfds);
		}
		// doesn't go in here if no clients are connected
		if (highest_socket > 0)
		{
			// checks if there are clients trying to send stuff
			if (select(highest_socket + 1, &readfds, NULL, NULL, &tv) > 0)
			{
				// going through all clients, checking whether each individual one is part of the
				// readfds set after it was changed by select (it'll only contain those who try to send things)
				// is there a more efficient ways to check which fds are in the set?
				// there must be a way to simply iterate through the set right?
				for (size_t i = 0; i < this->_clients.size(); i++)
				{
					if (FD_ISSET(this->_clients[i].getSocket(), &readfds))
					{
						// receives the message from the client, into the buffer, prints the message, sends it to all clients

						if (!recv(this->_clients[i].getSocket(), buffer, 1024, 0))
						{
							// if recv returns 0, it means that the client disconnected (always?)
							temp = "FD " + std::to_string(this->_clients[i].getSocket()) + " disconnected.\n";
							this->_clients.erase(i);
							for (size_t j = 0; j < this->_clients.size(); j++)
								send(this->_clients[j].getSocket(), temp.c_str(), temp.length(), 0);
						}
						else
						{
							// other return values (probably non negative though?) mean sent data
							temp = "FD " + std::to_string(this->_clients[i].getSocket()) + ": " + buffer;
							for (size_t j = 0; j < this->_clients.size(); j++)
							{
								if (j != i)
									send(this->_clients[j].getSocket(), temp.c_str(), temp.length(), 0);
							}
						}
						std::cout << temp;
						memset((void *)buffer, 0, 1024);
						temp = "";
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
	this->_address.sin_port = htons(8080);	//this'll be the passed port eventually
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
		exit (EXIT_FAILURE);
	}
	return (0);
}	

std::vector<Message> Server::parseMessages(char *input)
{
	std::vector<Message> msgs;
	std::string temp(input);
	size_t pos;
	while ((pos = temp.find('\n')) != temp.npos)
	{
		msgs.push_back(Message(temp.substr(0, temp.find('\n')-1)));
		temp = temp.substr(temp.find('\n') + 1, temp.npos);
	}
	for (size_t i = 0; i < msgs.size(); i++)
		std::cout << msgs[i] <<"$"<< std::endl;
	return (msgs);
}