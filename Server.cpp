/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmeixner <konstantin.meixner@freenet.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:58:19 by kmeixner          #+#    #+#             */
/*   Updated: 2022/10/07 13:18:49 by kmeixner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Server::Server()
{
}

Server::~Server()
{
}

int Server::init()
{
	this->_serverfd = socket(AF_INET, SOCK_STREAM, 0) q< 0)
}