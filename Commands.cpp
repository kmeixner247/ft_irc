#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>

// commands
void Server::PASS(Client *cl, Message msg)
{
	if (!msg.getParameters().back().compare(this->_password))
		cl->setPassbool(true);
	else
		cl->setPassbool(false);
	std::cout << "PASS " << std::endl;
	std::cout << msg << std::endl;
}
void Server::NICK(Client *cl, Message msg)
{
	//make nick lowercase in all cases
	// if there is a prefix; change user with nick prefix to parameter
	// if no prefix, introducing new nick for user
	std::cout << "NICK" << std::endl;
	std::cout << msg << std::endl;
	if (msg.getPrefix() != "")
		this->_registeredclients.erase(cl->getNickname());
	cl->setNickname(msg.getParameters().back());
	if (cl->getUsername() != "" && cl->getRealname() != "")
	{
	this->_registeredclients[cl->getNickname()] = cl;
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
	this->_registeredclients[cl->getNickname()] = cl;
		this->sendWelcome(cl);
	}

}

void Server::QUIT(Client *cl, Message msg)
{
	std::cout << "QUIT" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl);
}

void Server::KILL(Client *cl, Message msg)
{
	std::cout << "KILL" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::OPER(Client *cl, Message msg)
{
	std::cout << "OPER" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::SQUIT(Client *cl, Message msg)
{
	std::cout << "SQUIT" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::PRIVMSG(Client *cl, Message msg)
{
	std::cout << "PRIVMSG" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::WALLOPS(Client *cl, Message msg)
{
	std::cout << "WALLOPS" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::NOTICE(Client *cl, Message msg)
{
	std::cout << "NOTICE" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::KICK(Client *cl, Message msg)
{
	std::cout << "KICK" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::MODE(Client *cl, Message msg)
{
	std::cout << "MODE" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::INVITE(Client *cl, Message msg)
{
	std::cout << "INVITE" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::TOPIC(Client *cl, Message msg)
{
	std::cout << "TOPIC" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}