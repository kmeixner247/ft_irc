/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeixner <konstantin.meixner@freenet.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:56:13 by kmeixner          #+#    #+#             */
/*   Updated: 2022/10/07 12:22:30 by kmeixner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>

class Server
{
private:
	int _serverfd;
	struct sockaddr_in _address;

public:
	Server();
	~Server();

private:
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
};

#endif