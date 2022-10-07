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
	while (69)
	{
		if ((newfd = accept(this->_serverfd, (struct sockaddr *)&this->_address, (socklen_t *)&(addrlen))) > 0)
		{
			this->_clients.push_back(newfd);
			std::cerr << "WE GOT A NEW CLIENT FOLKS" << std::endl;
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