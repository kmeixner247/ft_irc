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

void Server::AWAY(Client *cl, Message msg)
{
	if (!msg.getParameters().size())
	{
		cl->removeMode(USERMODE_AWAY);
		this->sendMsg(cl, 1, RPL_UNAWAY(cl));
	}
	else
	{
		cl->addMode(USERMODE_AWAY);
		cl->setAwayMsg(msg.getParameters().front());
		this->sendMsg(cl, 1, RPL_NOWAWAY(cl));
	}
}

void Server::PASS(Client *cl, Message msg)
{
	if (msg.getParameters().size() == 0)
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "PASS"));
	else if (!msg.getParameters().back().compare(this->_password) && !cl->checkMode(USERMODE_PASS))
		cl->addMode(USERMODE_PASS);
	else if (this->clientIsRegistered(cl))
		this->sendMsg(cl, 1, ERR_ALREADYREGISTERED(cl));
	else
		cl->removeMode(USERMODE_PASS);
}

void Server::NICK(Client *cl, Message msg)
{
	// to avoid collisions in naming usernames are stored in lowercase
	// if there is a prefix; change user with nick prefix to parameter
	// if no prefix, introducing new nick for user
	if (msg.getParameters().size() == 0)
	{
		this->sendMsg(cl, 1, ERR_NONICKNAMEGIVEN(cl));
		return;
	}
	else if (!(strIsASCII(msg.getParameters().back())))
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
	if (msg.getParameters().size() < 4)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "USER"));
		return;
	}
	if (this->clientIsRegistered(cl))
	{
		this->sendMsg(cl, 1, ERR_ALREADYREGISTERED(cl));
		return;
	}
	cl->setUsername(msg.getParameters()[0]);
	if (msg.getParameters().at(3) == "")
		cl->setRealname("RealMcRealFace");
	else
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

	if (!msg.getParameters().size())
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "JOIN"));
		return ;
	}
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

	for (it = channelPW.begin(); it != channelPW.end(); it++)
	{
		if (it->first[0] != '#')
		{
			this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, it->first));
			continue ;
		}
		if (!this->_channels.count(it->first)) 
		{	//create channel, make client op
			newChan.setName(it->first);
			newChan.setKey(it->second);
			ch = this->addChannel(newChan);
			ch->addClient(&this->_bots.begin()->second);
			ch->addClient(cl);
			ch->addClientRight(cl, CHAN_OPERATOR);
			cl->addChannel(ch);
		}
		else
		{
			ch = &(this->_channels[it->first]);
			//ERR_BADCHANNELKEY
			if (ch->getKey() != "" && ch->getKey().compare(it->second) && !ch->isOnInviteList(makeNickMask(this, cl)))
			{
				this->sendMsg(cl, 1, ERR_BADCHANNELKEY(cl, ch));
				continue ;
			}
			//ERR_INVITEONLYCHAN
			if (ch->checkMode(CHANMODE_INVITE) && !ch->isOnInviteList(makeNickMask(this, cl)))
			{
				this->sendMsg(cl, 1, ERR_INVITEONLYCHAN(cl, ch));
				continue ;
			}
			//ERR_BANNED MCHAN
			if (ch->isBanned(makeNickMask(this, cl)) && !ch->isOnExcept(makeNickMask(this, cl)) && !ch->isOnInviteList(makeNickMask(this, cl)))
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
			if (ch->isOnInviteList(makeNickMask(this, cl)))
				ch->removeFromInviteList(makeNickMask(this, cl));
			ch->addClient(cl);
			cl->addChannel(ch);
		}
		this->sendMsg(ch, 1, JOINREPLY(cl, ch));
		if (ch->getTopic() != "")
			this->sendMsg(cl, 1, RPL_TOPIC(cl, ch).c_str());
		else
			this->sendMsg(cl, 1, RPL_NOTOPIC(cl, ch));
		this->sendMsg(cl, 1, RPL_NAMREPLY(cl, ch).c_str());
		this->sendMsg(cl, 1, RPL_ENDOFNAMES(cl, ch).c_str());
	}
}

void Server::QUIT(Client *cl, Message msg)
{
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.ChannelHasClient(cl))
		{
			if (msg.getParameters().size())
				this->sendMsg(&it->second, 1, QUITREPLY(cl, msg.getParameters().back()));
			else
				this->sendMsg(&it->second, 1, QUITREPLY(cl, ""));
			this->removeClientFromChannel(cl, &it->second);
			if (this->_channels.size() == 0)
				break ;
			it = this->_channels.begin();
		}
	}
	this->disconnectClient(cl);
}

void Server::WHO(Client *cl, Message msg)
{
	if (!msg.getParameters().size())
		return ;
	std::string mask = msg.getParameters().front();
	if (mask.front() == '#')
	{
		if (!this->_channels.count(mask))
			return ;
		Channel *ch = &this->_channels.at(mask);
		std::map<std::string, Client*> clients = ch->getClients();
		for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			this->sendMsg(cl, 1, RPL_WHOREPLY(cl, it->second));
	}
	else if (this->_registeredclients.count(mask))
		this->sendMsg(cl, 1, RPL_WHOREPLY(cl, this->_registeredclients.at(mask)));
	else
	{
		for (std::map<std::string, Client*>::iterator it = this->_registeredclients.begin(); it != this->_registeredclients.end(); it++)
		{
			if (matchMask(mask, makeNickMask(this, it->second)))
				this->sendMsg(cl, 1, RPL_WHOREPLY(cl, it->second));
		}
	}
	this->sendMsg(cl, 1, RPL_ENDOFWHO(cl, mask));
} 

void Server::KILL(Client *cl, Message msg)
{
	if (msg.getParameters().size() < 2)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "KILL"));
		return ;
	}
	if (msg.getParameters().front() == "behaviourbot")
		return ;
	if (!cl->checkMode(USERMODE_OP))
	{
		this->sendMsg(cl, 1, ERR_NOPRIVILEGES(cl));
		return ;
	}
	if (!this->_registeredclients.count(msg.getParameters().front()))
	{
		this->sendMsg(cl, 1, ERR_NOSUCHNICK(cl, msg.getParameters().front()));
		return ;
	}
	Client *target = this->_registeredclients.at(msg.getParameters().front());
	this->QUIT(target, msg);
}

void Server::PING(Client *cl, Message msg)
{
	if (!msg.getParameters().size())
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "PING"));
	else
		this->sendMsg(cl, 1, PONGREPLY(msg.getParameters().front()));
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
	if (cl->getNickname().compare(msg.getParameters()[0]) == 0)
	{
		cl->addMode(USERMODE_OP);
		this->sendMsg(cl, 1, RPL_YOUREOPER(cl).c_str());
		for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
			this->sendMsg(&it->second, 1, MODEREPLY(cl, it->second.getName(), "+o", cl->getNickname()));
	}
}

void Server::PRIVMSG(Client *cl, Message msg)
{
	Client *toCl;
	Channel *toCh;
	std::string target;
	std::string text;
	
	if (!msg.getParameters().size())
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "PRIVMSG"));
		return ;
	}
	for (size_t i = 0; i < msg.getParameters().size() - 1; i++)
	{
		target = msg.getParameters()[i];
		if (this->_bots.count(target))
			continue;
		if (target[0] == '#')
		{
			if (!this->_channels.count(target))
			{
				this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, target));
				continue ;
			}
			toCh = &this->_channels[target];
			if (toCh->isBanned(makeNickMask(this, cl)) && !toCh->isOnExcept(makeNickMask(this, cl)))
				continue ;
			if (!cl->ClientIsInChannel(toCh) && toCh->checkMode(CHANMODE_NOMSGFROMOUTSIDE))
			{
				this->sendMsg(cl, 1, ERR_CANNOTSENDTOCHAN(cl, toCh->getName()));
				continue ;
			}
			std::map<std::string, Client*> temptest = toCh->getClients();
			for (std::map<std::string, Client*>::iterator it = temptest.begin(); it != temptest.end(); it++)
			{
				text = msg.getParameters().back();
				if (it->second->getNickname() != cl->getNickname())
				{
					text = this->PRIVMSGREPLY(cl, toCh->getName(), text);
					
					std::cerr << "send channel ===> " << it->second->getNickname() << "!" << toCh->getName() << " : " << text << std::endl;
					send(it->second->getSocket(), text.c_str(), text.size(), 0);
				}
			}
			if (toCh->ChannelHasClient(&this->_bots.at("behaviourbot")))
			{
				if (this->_karen->checkBehaviour(text))
				{
					this->sendMsg(toCh, 1, KICKREPLY(&this->_bots["behaviourbot"], toCh, cl, "I'M A CHATMAN Bi-Ba-Ba-Ba-Ba-Bob https://www.youtube.com/watch?v=Hy8kmNEo1i8"));
					this->removeClientFromChannel(cl, toCh);
				}
			}
		}
		else
		{
			text = msg.getParameters().back();
			if (!this->_registeredclients.count(target))
			{
				this->sendMsg(cl, 1, ERR_NOSUCHNICK(cl, target));
				continue ;
			}
			toCl = this->_registeredclients[target];
			if (toCl->checkMode(USERMODE_AWAY))
				this->sendMsg(cl, 1, this->RPL_AWAY(cl, toCl));
			this->sendMsg(toCl, 1, this->PRIVMSGREPLY(cl, toCl->getNickname(), text));
		}
	}
}

void Server::NOTICE(Client *cl, Message msg)
{
	Client *toCl;
	Channel *toCh;
	std::string text;

	if (!msg.getParameters().size())
		return ;
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
		}
		else
		{
			toCl = this->_registeredclients[msg.getParameters()[i]];
			this->sendMsg(toCl, 1, this->NOTICEREPLY(cl, toCl->getNickname(), text));
		}
	}
}

void Server::KICK(Client *cl, Message msg)
{
	
	if (msg.getParameters().size() < 2)
	{
		this->sendMsg(cl, 1, this->ERR_NEEDMOREPARAMS(cl, "KICK"));
		return ;
	}
	std::string targetnick = msg.getParameters()[1];
	if (targetnick.back() == ',')
		targetnick.resize(targetnick.size() - 1);
	if (!this->_channels.count(msg.getParameters().front()))
	{
		this->sendMsg(cl, 1, this->ERR_NOSUCHCHANNEL(cl, msg.getParameters().front()));
		return ;
	}
	Channel *ch = &this->_channels.at(msg.getParameters().front());
	if (!ch->checkClientRight(cl, CHAN_OPERATOR) && !cl->checkMode(USERMODE_OP))
	{
		this->sendMsg(cl, 1, ERR_CHANOPRIVSNEEDED(cl, ch));
		return ;
	}
	if (!ch->ChannelHasClient(cl))
	{
		this->sendMsg(cl, 1, ERR_NOTONCHANNEL(cl, ch->getName()));
		return ;
	}
	Client *target;
	target = ch->getClient(targetnick);
	if (!target)
	{
		this->sendMsg(cl, 1, ERR_USERNOTINCHANNEL(cl, targetnick, ch->getName()));
		return ;
	}
	std::string comment = (msg.getParameters().back()[0] == ':') ? msg.getParameters().back() : "";
	this->sendMsg(ch, 1, KICKREPLY(cl, ch, target, comment));
	this->removeClientFromChannel(target, ch);
}

void Server::MODE(Client *cl, Message msg)
{

	if (!msg.getParameters().size())
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
		std::pair<std::string, bool> changedmodes = cl->changeModes(msg.getParameters()[1]);
		if (changedmodes.first != "")
			this->sendMsg(cl, 1, this->MODEREPLY(cl, cl->getNickname(), changedmodes.first, msg.getParameters()));
		if (changedmodes.second)
			this->sendMsg(cl, 1, this->ERR_UMODEUNKNOWNFLAG(cl));
	}
	else
	{
		if (!this->_channels.count(target))
		{
			this->sendMsg(cl, 1, this->ERR_NOSUCHCHANNEL(cl, target));
			return ;
		}
		Channel *ch = &this->_channels[target];
		if (msg.getParameters().size() == 1)
		{
			this->sendMsg(cl, 1, this->RPL_CHANNELMODEIS(cl, ch));
			return ;
		}
		if (!ch->checkClientRight(cl, CHAN_OPERATOR) && !ch->checkMode(USERMODE_OP))
		{
			this->sendMsg(cl, 1, this->ERR_CHANOPRIVSNEEDED(cl, ch));
			return ;
		}
		std::pair<std::string, bool> changedmodes = ch->changeModes(msg.getParameters());
		if (changedmodes.first != "")
			this->sendMsg(ch, 1, this->MODEREPLY(cl, ch->getName(), changedmodes.first, msg.getParameters()));
		if (changedmodes.second)
			this->sendMsg(cl, 1, this->ERR_UMODEUNKNOWNFLAG(cl));
	}
}

void Server::INVITE(Client *cl, Message msg)
{
	
	if (msg.getParameters().size() < 2)
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "INVITE"));
		return ;
	}
	std::string channel = msg.getParameters().back();
	if (!this->_channels.count(channel))
	{
		this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, channel));
		return ;
	}
	Channel *ch = &this->_channels.at(channel);
	if (!cl->ClientIsInChannel(ch))
	{
		this->sendMsg(cl, 1, ERR_NOTONCHANNEL(cl, channel));
		return ;
	}
	if (ch->checkMode(CHANMODE_INVITE) && !ch->checkClientRight(cl, CHAN_OPERATOR))
	{
		this->sendMsg(cl, 1, ERR_CHANOPRIVSNEEDED(cl, ch));
		return ;
	}
	if (!this->_registeredclients.count(msg.getParameters().front()))
	{
		this->sendMsg(cl, 1, ERR_NOSUCHNICK(cl, msg.getParameters().front()));
		return ;
	}
	Client *target = this->_registeredclients.at(msg.getParameters().front());
	if (ch->ChannelHasClient(target))
	{
		this->sendMsg(cl, 1, ERR_USERONCHANNEL(cl, ch, msg.getParameters().front()));
		return ;
	}
	std::string temp = makeNickMask(this, target);
	ch->addToInviteList(makeNickMask(this, target));
	this->sendMsg(cl, 1, RPL_INVITING(cl, msg.getParameters().front(), msg.getParameters().back()));
	this->sendMsg(target, 1, INVITEREPLY(target, ch, cl));
}

void Server::TOPIC(Client *cl, Message msg)
{
	if (!msg.getParameters().size())
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "TOPIC"));
		return ;
	}
	if (!this->_channels.count(msg.getParameters().front()))
	{
		this->sendMsg(cl, 1, ERR_NOSUCHCHANNEL(cl, msg.getParameters().front()));
		return ;
	}
	Channel *ch = &this->_channels.at(msg.getParameters().front());
	if (!ch->ChannelHasClient(cl))
	{
		this->sendMsg(cl, 1, ERR_NOTONCHANNEL(cl, ch->getName()));
		return ;
	}
	if (msg.getParameters().size() == 1)
	{
		if (ch->getTopic() == "")
			this->sendMsg(cl, 1, RPL_NOTOPIC(cl, ch));
		else
			this->sendMsg(cl, 1, RPL_TOPIC(cl, ch));
	}
	else
	{
		if (ch->checkMode(CHANMODE_TOPIC) && !ch->checkClientRight(cl, CHAN_OPERATOR))
			this->sendMsg(cl, 1, ERR_CHANOPRIVSNEEDED(cl, ch));
		else
		{
			ch->setTopic(msg.getParameters()[1]);
			this->sendMsg(ch, 1, TOPICREPLY(cl, ch, msg.getParameters()[1]));
		}
	}
}

void Server::PART(Client *cl, Message msg)
{
	Channel *ch;
	std::string reason;
	std::string name;
	if (!msg.getParameters().size())
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
		}
		channels = channels.substr(pos+1, channels.back());
	}
	while (pos != std::string::npos);
}

void Server::WALLOPS(Client *cl, Message msg)
{
	if (!msg.getParameters().size())
	{
		this->sendMsg(cl, 1, ERR_NEEDMOREPARAMS(cl, "WALLOPS"));
		return ;
	}
	if (!cl->checkMode(USERMODE_OP))
	{
		this->sendMsg(cl, 1, ERR_NOPRIVILEGES(cl));
		return ;
	}
	for (std::map<std::string, Client *>::iterator it = this->_registeredclients.begin(); it != this->_registeredclients.end(); it++)
	{
		if ((*it).second->checkMode(USERMODE_WALLOPRECEIVER))
			this->sendMsg(it->second, 1, NOTICEREPLY(cl, it->second->getNickname(), msg.getParameters().front())); 
	}
}

void Server::DIE(Client *cl)
{
	if (cl->checkMode(USERMODE_OP))
		Server::s_active = false;
}

void Server::ADDBAD(Client *cl, Message msg)
{
	if (!cl->checkMode(USERMODE_OP))
		return ;
	std::vector<std::string> words = msg.getParameters();
	std::string text;
	size_t count = 0;
	text += cl->getNickname() + " added the word";
	if (words.size() > 1)
		text += "s";
	text += " ( ";
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if (this->_karen->addBadWord(*it))
		{
			text += *it + " ";
			count++;
		}
	}
	if (!count)
		return;
	text.resize(text.size() - 1);
	text += " ) to the BAD word pool.";
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.ChannelHasClient(this->_karen->getBotClient()))
			this->sendMsg(&it->second, 1, this->BOTREPLY(it->second.getName(), text));
	}
}

void Server::RMBAD(Client *cl, Message msg)
{
	if (!cl->checkMode(USERMODE_OP))
		return ;
	std::vector<std::string> words = msg.getParameters();
	std::string text;
	size_t count = 0;
	text += cl->getNickname() + " removed the word";
	if (words.size() > 1)
		text += "s";
	text += " ( ";
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if (this->_karen->removeBadWord(*it))
		{
			text += *it + " ";
			count++;
		}
	}
	if (!count)
		return;
	text.resize(text.size() - 1);
	text += " ) from the BAD word pool.";
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.ChannelHasClient(this->_karen->getBotClient()))
			this->sendMsg(&it->second, 1, this->BOTREPLY(it->second.getName(), text));
	}
}
void Server::ADDGOOD(Client *cl, Message msg)
{
	if (!cl->checkMode(USERMODE_OP))
		return ;
	std::vector<std::string> words = msg.getParameters();
	std::string text;
	size_t count = 0;
	text += cl->getNickname() + " added the word";
	if (words.size() > 1)
		text += "s";
	text += " ( ";
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if (this->_karen->addGoodWord(*it))
		{
			text += *it + " ";
			count++;
		}
	}
	if (!count)
		return;
	text.resize(text.size() - 1);
	text += " ) to the GOOD word pool.";
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.ChannelHasClient(this->_karen->getBotClient()))
			this->sendMsg(&it->second, 1, this->BOTREPLY(it->second.getName(), text));
	}
}

void Server::RMGOOD(Client *cl, Message msg)
{
	if (!cl->checkMode(USERMODE_OP))
		return ;
	std::vector<std::string> words = msg.getParameters();
	std::string text;
	size_t count = 0;
	text += cl->getNickname() + " removed the word";
	if (words.size() > 1)
		text += "s";
	text += " ( ";
	for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); it++)
	{
		if (this->_karen->removeGoodWord(*it))
		{
			text += *it + " ";
			count++;
		}
	}
	if (!count)
		return;
	text.resize(text.size() - 1);
	text += " ) from the GOOD word pool.";
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.ChannelHasClient(this->_karen->getBotClient()))
			this->sendMsg(&it->second, 1, this->BOTREPLY(it->second.getName(), text));
	}
}

void Server::BHVLIST(Client *cl)
{
	this->sendMsg(cl, 1, BHVLISTREPLY(cl, this->_karen->getGoodWords()));
	this->sendMsg(cl, 1, BHVLISTREPLY(cl, this->_karen->getBadWords()));
}