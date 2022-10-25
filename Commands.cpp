#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>

std::string strToLower(std::string str)
{
	for (int i = 0; str[i] != '\0'; i++)
		str[i] = (char)std::tolower(str[i]);
	return (str);
}

bool strIsASCII(const std::string& str)
{
	for (int i = 0; str[i] != '\0'; i++)
		if (!isascii(str[i]))
			return(false);
	return (true);
}

// commands
void Server::PASS(Client *cl, Message msg)
{
	if (msg.getParameters().size() == 0)
	{
		this->sendResponse(cl, ERR_NEEDMOREPARAMS);
		this->sendResponse(cl, "\n");
	}
	else if (!msg.getParameters().back().compare(this->_password) && !cl->getPassbool())
		cl->setPassbool(true);
	else if (this->clientIsRegistered(cl)) //check, if adium/pidgin enter here when passbool is true, but cl not registered
	{
		this->sendResponse(cl, ERR_ALREADYREGISTRED);
		this->sendResponse(cl, "\n"); 
	}
	else
		cl->setPassbool(false);
	std::cout << "PASS " << std::endl;
	std::cout << msg << std::endl;
}

void Server::NICK(Client *cl, Message msg)
{
	// to avoid collisions in naming usernames are stored in lowercase
	// if there is a prefix; change user with nick prefix to parameter
	// if no prefix, introducing new nick for user
	std::cout << "NICK" << std::endl;
	std::cout << msg << std::endl;
	if (msg.getParameters().size() == 0)
	{
		this->sendResponse(cl, ERR_NONICKNAMEGIVEN);
		this->sendResponse(cl, "\n");
		return;
	}
	else if (!(strIsASCII(msg.getParameters().back()))) //check with ref client if it is really ascii
	{
		this->sendResponse(cl, ERR_ERRONEUSNICKNAME);
		this->sendResponse(cl, "\n");
		return;
	}
	else if (this->_registeredclients.find(strToLower(msg.getParameters().back())) != this->_registeredclients.end())
	{
		this->sendResponse(cl, ERR_NICKNAMEINUSE);
		this->sendResponse(cl, "\n");
		return;
	}
	std::string prefixLower = strToLower(msg.getPrefix());
	std::string paramLower = strToLower(msg.getParameters().back());
	if (prefixLower != "" && (prefixLower.compare(cl->getNickname())))
	{
		this->_registeredclients.erase(cl->getNickname());
		this->_registeredclients.insert(std::make_pair(paramLower, cl));
		cl->setNickname(paramLower);
		return;
	}
	if (cl->getUsername() != "" && cl->getRealname() != "")
	{
		if (prefixLower == "" && this->_registeredclients.find(paramLower) == this->_registeredclients.end())
		{
			cl->setNickname(paramLower);
			this->_registeredclients[cl->getNickname()] = cl;
			this->sendWelcome(cl);
			return;
		}
		else return;

	}
	if (cl->getUsername() == "" && cl->getRealname() == "")
		cl->setNickname(paramLower);
}

void Server::USER(Client *cl, Message msg)
{
	std::cout << "USER" << std::endl;
	std::cout << msg << std::endl;
	if (this->clientIsRegistered(cl))
	{
		this->sendResponse(cl, ERR_ALREADYREGISTRED);
		this->sendResponse(cl, "\n");
		return;
	}
	else if (msg.getParameters().size() < 4 || msg.getParameters().at(3) == "")//check with Pidgin/Adim if that is really the right min amount of params
	{
		this->sendResponse(cl, ERR_NEEDMOREPARAMS);
		this->sendResponse(cl, "\n");
		return;
	}
	cl->setUsername(msg.getParameters()[0]);
	cl->setRealname(msg.getParameters().back());
	if (cl->getNickname() != "")
	{
	this->_registeredclients[cl->getNickname()] = cl;
		this->sendWelcome(cl);
	}

}

void Server::JOIN(Client *cl, Message msg)
{
	Channel *ch;
	std::cout << "JOIN" << std::endl;
	std::cout << msg << std::endl;

	/* !!!CAN'T HANDLE MULTIPLE CHANNELS YET!!! */

	/*	NEED:
		Client::addChannel
		ERR_NEEDMOREPARAMS
		ERR_BADCHANMASK
		ERR_NOSUCHCHANNEL
		ERR_BADCHANNELKEY
		ERR_INVITEONLYCHAN
		ERR_BANNEDFROMCHAN
		ERR_CHANNELISFULL
		RPL_TOPIC
		RPL_NAMREPLY
		RPL_ENDOFNAMES
	*/

	//ERR_NEEDMOREPARAMS
	if (msg.getParameters().size() == 0)
	{
		this->sendResponse(cl, ERR_NEEDMOREPARAMS);
		return ;
	}
	if (msg.getParameters()[0][0] != '#')
	{
		this->sendResponse(cl, ERR_NOSUCHCHANNEL);
		return ;
	}
	if (!this->_channels.count(msg.getParameters().front())) 
	{	//create channel, make client op
		Channel newchan(msg.getParameters()[0]); //assuming name constructor
		ch = &newchan;
		this->addChannel(ch);
		ch->addClient(cl);

		ch->addClientRight(cl, CHAN_OPERATOR);
		cl->addChannel(ch);
	}
	else
	{

		ch = this->_channels[msg.getParameters()[0]];
		std::cerr << ch->getName() << std::endl;
		//ERR_BADCHANNELKEY
		if (ch->getPrivateChan() && ch->getKey().compare(msg.getParameters().back()))
		{
			this->sendResponse(cl, ERR_BADCHANNELKEY);
			return ;
		}
		//ERR_INVITEONLYCHAN
		if (ch->getInviteOnly() && ch->checkClientRight(cl, CHAN_INVITE))
		{
			this->sendResponse(cl, ERR_INVITEONLYCHAN);
			return ;
		}
		//ERR_BANNEDFROMCHAN
		if (ch->checkClientRight(cl, CHAN_BAN))
		{
			this->sendResponse(cl, ERR_BANNEDFROMCHAN);
			return ;
		}
		//ERR_CHANNELISFULL 
		if (ch->getLimit() >= ch->getSize())
		{
			this->sendResponse(cl, ERR_CHANNELISFULL);
			return ;
		}
		ch->addClient(cl);
		cl->addChannel(ch);
		//noticess about commands?
	}
	std::cerr << "sending channel welcome" << std::endl;
	this->sendResponse(cl, RPL_TOPIC);
	this->sendResponse(cl, RPL_NAMREPLY);
	this->sendResponse(cl, RPL_ENDOFNAMES);
	
}

void Server::QUIT(Client *cl, Message msg)
{
	std::cout << "QUIT" << std::endl;
	std::cout << msg << std::endl;
	this->disconnectClient(cl);
}

void Server::WHO(Client *cl, Message msg)
{
	std::cout << "WHO" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "My nickname is " << cl->getNickname() << std::endl;
	std::cout << "My realname is " << cl->getRealname() << std::endl;
	std::cout << "My username is " << cl->getUsername() << std::endl;
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