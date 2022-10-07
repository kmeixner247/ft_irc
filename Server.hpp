/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeixner <konstantin.meixner@freenet.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:56:13 by kmeixner          #+#    #+#             */
/*   Updated: 2022/10/07 14:06:56 by kmeixner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Server
{
private:
	int _serverfd;
	struct sockaddr_in _address;
	std::vector<int> _clients;

public:
	Server();
	~Server();

private:
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
};

#endif