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
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "PASS"));
	else if (!msg.getParameters().back().compare(this->_password) && !cl->getPassbool())
		cl->setPassbool(true);
	else if (this->clientIsRegistered(cl)) //check, if adium/pidgin enter here when passbool is true, but cl not registered
		this->sendMsg(cl, 1, ERR_ALREADYREGISTERED(cl));
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
		this->sendMsg(cl, 1, ERR_NONICKNAMEGIVEN(cl));
		return;
	}
	else if (!(strIsASCII(msg.getParameters().back()))) //check with ref client if it is really ascii
	{
		this->sendMsg(cl, 1, ERR_ERRONEUSNICKNAME(cl));
		return;
	}
	else if (this->_registeredclients.find(strToLower(msg.getParameters().back())) != this->_registeredclients.end())
	{
		this->sendMsg(cl, 1, ERR_NICKNAMEINUSE(cl));
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
			this->sendMsg(cl, 3, RPL_MOTDSTART(cl).c_str(), RPL_MOTD(cl).c_str(), RPL_ENDOFMOTD(cl).c_str());
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
		this->sendMsg(cl, 1, ERR_ALREADYREGISTERED(cl));
		// this->sendResponse(cl, ERR_ALREADYREGISTRED);
		return;
	}
	else if (msg.getParameters().size() < 4 || msg.getParameters().at(3) == "")//check with Pidgin/Adim if that is really the right min amount of params
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "USER"));
		// this->sendResponse(cl, ERR_NEEDMOREPARAMS);
		return;
	}
	cl->setUsername(msg.getParameters()[0]);
	cl->setRealname(msg.getParameters().back());
	if (cl->getNickname() != "")
	{
	this->_registeredclients[cl->getNickname()] = cl;
		this->sendMsg(cl, 3, RPL_MOTDSTART(cl).c_str(), RPL_MOTD(cl).c_str(), RPL_ENDOFMOTD(cl).c_str());
	}

}

void Server::JOIN(Client *cl, Message msg)
{
	Channel newchan;
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
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "JOIN"));
		// this->sendResponse(cl, ERR_NEEDMOREPARAMS);
		return ;
	}
	//better check for invalid channel names and fix sending the invalid channel name (there is no channel object obviously)
	if (msg.getParameters()[0][0] != '#')
	{
		std::cout << "sending nosuchchannel" << std::endl;
		// this->sendResponse(cl, ERR_NOSUCHCHANNEL);
		this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, msg.getParameters()[0]));
		return ;
	}
	if (!this->_channels.count(msg.getParameters()[0])) 
	{	//create channel, make client op
		newchan.setName(msg.getParameters()[0]);
		ch = this->addChannel(newchan);
		ch->addClient(cl);
		ch->addClientRight(cl, CHAN_OPERATOR);
		cl->addChannel(ch);
	}
	else
	{
		ch = &(this->_channels[msg.getParameters()[0]]);
		//ERR_BADCHANNELKEY
		if (ch->getPrivateChan() && ch->getKey().compare(msg.getParameters().back()))
		{
			this->sendMsg(cl, 1, ERR_BADCHANNELKEY(cl, ch));
			// this->sendResponse(cl, ERR_BADCHANNELKEY);
			return ;
		}
		//ERR_INVITEONLYCHAN
		if (ch->getInviteOnly() && ch->checkClientRight(cl, CHAN_INVITE))
		{
			this->sendMsg(cl, 1, ERR_INVITEONLYCHAN(cl, ch));
			return ;
		}
		//ERR_BANNED MCHAN
		if (ch->checkClientRight(cl, CHAN_BAN))
		{
			this->sendMsg(cl, 1, ERR_BANNEDFROMCHAN(cl, ch));
			return ;
		}
		//ERR_CHANNELISFULL 
		if (ch->getLimit() <= ch->getSize())
		{
			this->sendMsg(cl, 1, ERR_CHANNELISFULL(cl, ch));
			return ;
		}
		ch->addClient(cl);
		cl->addChannel(ch);
	}
	//send JOIN with nick as prefix to channel)
	// this->sendResponse(cl, ch, ":<nick> JOIN #test\r\n");
	this->sendMsg(ch, 1, JOINREPLY(cl, ch));
	this->sendMsg(cl, 3, RPL_TOPIC(cl, ch).c_str(), RPL_NAMREPLY(cl, ch).c_str(), RPL_ENDOFNAMES(cl, ch).c_str());
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
	std::cout << "KILL from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::OPER(Client *cl, Message msg)
{
	std::cout << "OPER from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::SQUIT(Client *cl, Message msg)
{
	std::cout << "SQUIT from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::PRIVMSG(Client *cl, Message msg)
{
	std::cout << "PRIVMSG from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::WALLOPS(Client *cl, Message msg)
{
	std::cout << "WALLOPS from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::NOTICE(Client *cl, Message msg)
{
	std::cout << "NOTICE from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::KICK(Client *cl, Message msg)
{
	std::cout << "KICK from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::MODE(Client *cl, Message msg)
{
	std::cout << "MODE from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::INVITE(Client *cl, Message msg)
{
	std::cout << "INVITE from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::TOPIC(Client *cl, Message msg)
{
	std::cout << "TOPIC from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}