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
	else if (!msg.getParameters().back().compare(this->_password) && !cl->checkMode(USERMODE_PASS))
		cl->addMode(USERMODE_PASS);
	else if (this->clientIsRegistered(cl)) //check, if adium/pidgin enter here when passbool is true, but cl not registered
		this->sendMsg(cl, 1, ERR_ALREADYREGISTERED(cl));
	else
		cl->removeMode(USERMODE_PASS);
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
	Channel newChan;
	Channel *ch;
	std::vector<std::pair<std::string, std::string> > channelPW;
	std::vector<std::pair<std::string, std::string> >::iterator it;
	size_t pos;
	std::string temp = msg.getParameters().front();
	do
	{
		pos = temp.find_first_of(',');
		channelPW.push_back(std::make_pair(temp.substr(0, pos), ""));
		temp = temp.substr(pos+1, temp.back());
	}
	while (pos != std::string::npos);
	if (msg.getParameters().size() > 1)
	{
		temp = msg.getParameters().back();
		it = channelPW.begin();
		do
		{
			pos = temp.find_first_of(',');
			it->second = temp.substr(0, pos);
			temp = temp.substr(pos+1, temp.back());
			it++;
		}
		while (pos != std::string::npos);
	}
	/* !!!CAN'T HANDLE MULTIPLE CHANNELS YET!!! */

	//ERR_NEEDMOREPARAMS
	if (msg.getParameters().size() == 0)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "JOIN"));
		return ;
	}
	for (it = channelPW.begin(); it != channelPW.end(); it++)
	{
		std::cerr << "CURRENT CHANNEL KEY PAIR IS " << "|" << it->first << "|" << it->second << "|" <<  std::endl;
		if (it->first[0] != '#')
		{
			std::cout << "sending nosuchchannel" << std::endl;
			this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, it->first));
			continue ;
		}
		if (!this->_channels.count(it->first)) 
		{	//create channel, make client op
			newChan.setName(it->first);
			std::cerr << "XXXXXXXXXXXXXXXXXXXX" << it->second << "XXXXXXXXXXXXXXXXXXXX" << std::endl;
			newChan.setKey(it->second);
			ch = this->addChannel(newChan);
			ch->addClient(cl);
			ch->addClientRight(cl, CHAN_OPERATOR);
			cl->addChannel(ch);
		}
		else
		{
			ch = &(this->_channels[it->first]);
			std::cerr << "KEY|" <<  ((ch->getKey() != "") ? ch->getKey() : "KEY IS EMPTY") << "|" << std::endl;
			//ERR_BADCHANNELKEY
			if (ch->getKey() != "" && ch->getKey().compare(it->second))
			{
				this->sendMsg(cl, 1, ERR_BADCHANNELKEY(cl, ch));
				continue ;
			}
			//ERR_INVITEONLYCHAN
			if (ch->checkMode(CHANMODE_INVITE) && ch->checkClientRight(cl, CHAN_INVITE))
			{
				this->sendMsg(cl, 1, ERR_INVITEONLYCHAN(cl, ch));
				continue ;
			}
			//ERR_BANNED MCHAN
			if (ch->checkClientRight(cl, CHAN_BAN))
			{
				this->sendMsg(cl, 1, ERR_BANNEDFROMCHAN(cl, ch));
				continue ;
			}
			//ERR_CHANNELISFULL 
			if (ch->checkMode(CHANMODE_LIMIT) && ch->getLimit() <= ch->getSize())
			{
				this->sendMsg(cl, 1, ERR_CHANNELISFULL(cl, ch));
				continue ;
			}
			ch->addClient(cl);
			cl->addChannel(ch);
		}
		//send JOIN with nick as prefix to channel)
		this->sendMsg(ch, 1, JOINREPLY(cl, ch));
		this->sendMsg(cl, 1, RPL_TOPIC(cl, ch).c_str());
		this->sendMsg(cl, 1, RPL_NAMREPLY(cl, ch).c_str());
		this->sendMsg(cl, 1, RPL_ENDOFNAMES(cl, ch).c_str());
	}
}

void Server::QUIT(Client *cl, Message msg)
{
	std::cout << "QUIT" << std::endl;
	std::cout << msg << std::endl;

	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.getClients().count(cl->getNickname()))
		{
			it->second.getClients().erase(cl->getNickname());
			this->sendMsg(&it->second, 1, QUITREPLY(cl, msg.getParameters().back()));
		}
	}
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
	if (msg.getParameters().size() < 2)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "OPER"));
		return ;
	}
	if (this->getPasswordOper().compare(msg.getParameters()[1]) != 0)
	{	
		this->sendMsg(cl, 1, ERR_PASSWDMISMATCH(cl));
		return ;
	}
	if (cl->getUsername().compare(msg.getParameters()[0]) == 0)
	{
		cl->addMode(USERMODE_OP);
		this->sendMsg(cl, 1, RPL_YOUREOPER(cl).c_str());
	}
	
	// MODE MSG NEEDED
}

void Server::SQUIT(Client *cl, Message msg)
{
	std::cout << "SQUIT from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
	// this->disconnectClient(cl); //PLACEHOLDER TO BE REPLACED
}

void Server::PRIVMSG(Client *cl, Message msg)
{
	Client *toCl;
	Channel *toCh;
	std::string text;
	std::cout << "PRIVMSG from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	
    // ERR_NOSUCHNICK (401)
    // ERR_NOSUCHSERVER (402)
    // ERR_CANNOTSENDTOCHAN (404)
    // ERR_TOOMANYTARGETS (407)
    // ERR_NORECIPIENT (411)
    // ERR_NOTEXTTOSEND (412)
    // ERR_NOTOPLEVEL (413)
    // ERR_WILDTOPLEVEL (414)
    // RPL_AWAY (301)

	text = msg.getParameters().back();
	for (size_t i = 0; i < msg.getParameters().size() - 1; i++)
	{

		if (msg.getParameters()[i][0] == '#')
		{
			toCh = &this->_channels[msg.getParameters()[i]];
			std::map<std::string, Client*> temptest = toCh->getClients();
			for (std::map<std::string, Client*>::iterator it = temptest.begin(); it != temptest.end(); it++)
			{
				if (it->second->getNickname() != cl->getNickname())
				{
					text = this->PRIVMSGREPLY(cl, toCh->getName(), text);
					send(it->second->getSocket(), text.c_str(), text.size(), 0);
				}
			}
			// this->sendMsg(toCh, 1, this->PRIVMSGREPLY(cl, toCh->getName(), text));
		}
		else
		{
			toCl = this->_registeredclients[msg.getParameters()[i]];
			this->sendMsg(toCl, 1, this->PRIVMSGREPLY(cl, toCl->getNickname(), text));
		}
	}

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
	Client *toCl;
	Channel *toCh;
	std::string text;
	std::cout << "NOTICE from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;

	text = msg.getParameters().back();
	for (size_t i = 0; i < msg.getParameters().size() - 1; i++)
	{

		if (msg.getParameters()[i][0] == '#')
		{
			toCh = &this->_channels[msg.getParameters()[i]];
			std::map<std::string, Client*> temptest = toCh->getClients();
			for (std::map<std::string, Client*>::iterator it = temptest.begin(); it != temptest.end(); it++)
			{
				if (it->second->getNickname() != cl->getNickname())
				{
					text = this->NOTICEREPLY(cl, toCh->getName(), text);
					send(it->second->getSocket(), text.c_str(), text.size(), 0);
				}
			}
			// this->sendMsg(toCh, 1, this->PRIVMSGREPLY(cl, toCh->getName(), text));
		}
		else
		{
			toCl = this->_registeredclients[msg.getParameters()[i]];
			this->sendMsg(toCl, 1, this->NOTICEREPLY(cl, toCl->getNickname(), text));
		}
	}

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

	if (msg.getParameters().size() == 0)
	{
		this->sendMsg(cl, 1, this->ERR_NEEDMOREPARAMS(cl, "MODE"));
		return ;
	}
	std::string target = msg.getParameters().front();
	if (target[0] != '#')
	{
		if (!this->_registeredclients.count(target))
		{
			this->sendMsg(cl, 1, this->ERR_NOSUCHNICK(cl, target));
			return ;
		}
		if (target != cl->getNickname())
		{
			this->sendMsg(cl, 1, this->ERR_USERSDONTMATCH(cl, target));
			return ;
		}
		if (msg.getParameters().size() == 1)
		{
			this->sendMsg(cl, 1, this->RPL_UMODEIS(cl));
			return ;
		}
	}
	else
	{
		if (!this->_channels.count(target))
		{
			this->sendMsg(cl, 1, this->ERR_NOSUCHCHANNEL(cl, target));
			return ;
		}
		if (msg.getParameters().size() == 1)
		{
			this->sendMsg(cl, 1, this->RPL_CHANNELMODEIS(cl, &this->_channels[target]));
			return ;
		}
	}
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

void Server::PART(Client *cl, Message msg)
{
	std::cout << "PART from " << cl->getNickname() << std::endl;
	std::cout << msg << std::endl;
	Channel *ch;
	std::string reason;
	std::string name;
	if (msg.getParameters().size() == 0)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "PART"));
		return ;
	}
	reason = (msg.getParameters().size() > 1 ? msg.getParameters().back() : "");
	std::string channels = msg.getParameters().front();
	size_t pos;
	do
	{
		pos = channels.find_first_of(',');
		name = channels.substr(0, pos);
		if (!this->_channels.count(name))
			this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, name));
		else if (!this->_channels[name].getClients().count(cl->getNickname()))
			this->sendMsg(cl, 1, ERR_NOTONCHANNEL(cl, name));
		else
		{
			ch = &this->_channels[channels.substr(0, pos)];
			this->sendMsg(ch, 1, this->PARTREPLY(cl, ch->getName(), reason));
			this->removeClientFromChannel(cl, ch);
			// this->sendMsg(cl, 1, this->PARTREPLY(cl, ch->getName(), reason));
			//leave_channel(channels.substr(0, pos));
			// std::cerr << channels.substr(0, pos) << std::endl;
		}
		channels = channels.substr(pos+1, channels.back());
	}
	while (pos != std::string::npos);
}