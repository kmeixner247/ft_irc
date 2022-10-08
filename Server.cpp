/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeixner <konstantin.meixner@freenet.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:58:19 by kmeixner          #+#    #+#             */
/*   Updated: 2022/10/07 14:25:32 by kmeixner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			// saving the client fd in a vector
			// probably better as a map<int><Client> with a Client object later
			std::cout << "FD " << newfd << " connected." << std::endl;
			temp = "FD " + std::to_string(newfd) + " connected.\n";
			for (size_t j = 0; j < this->_clients.size(); j++)
					send(this->_clients[j], temp.c_str(), temp.length(), 0);
			this->_clients.push_back(newfd);
			// making the client fd nonblocking (is this necessary?)
			if (fcntl(newfd, F_SETFL, O_NONBLOCK) == -1)
			{
				perror("fcntl failed");
				exit (EXIT_FAILURE);
			}
		}
		// clearing the readfds set (necessary?)
		FD_ZERO(&readfds);
		highest_socket = -1;
		// filling the readfds set (for select) while finding the highest socket fd (also for select)
		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			if (this->_clients[i] > highest_socket)
				highest_socket = this->_clients[i];
			FD_SET(this->_clients[i], &readfds);
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
					if (FD_ISSET(this->_clients[i], &readfds))
					{
						// receives the message from the client, into the buffer, prints the message, sends it to all clients

						if (!recv(this->_clients[i], buffer, 1024, 0))
						{
							// if recv returns 0, it means that the client disconnected (always?)
							temp = "FD " + std::to_string(this->_clients[i]) + " disconnected.\n";
							this->_clients.erase(this->_clients.begin() + i);
							for (size_t j = 0; j < this->_clients.size(); j++)
								send(this->_clients[j], temp.c_str(), temp.length(), 0);
						}
						else
						{
							// other return values (probably non negative though?) mean sent data
							temp = "FD " + std::to_string(this->_clients[i]) + ": " + buffer;
							for (size_t j = 0; j < this->_clients.size(); j++)
							{
								if (j != i)
									send(this->_clients[j], temp.c_str(), temp.length(), 0);
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
	this->_address.sin_port = htons(8080);
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