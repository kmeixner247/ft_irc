#include "Channel.hpp"

Channel::Channel() : 
	_name(""), 
	_clients(std::map<std::string, Client *>()),
	_chanmodes(512),
	_topic(""), 
	_limit(99999), 
	_key(""),
	_clientRights(std::map<std::string, int>())
{
}
Channel::Channel(const std::string &name) : 
	_name(name), 
	_clients(std::map<std::string, Client *>()),
	_chanmodes(512),
	_topic(""), 
	_limit(99999), 
	_key(""),
	_clientRights(std::map<std::string, int>())
{
}

Channel::Channel(const Channel &rhs)
{
	*this = rhs;
}

Channel::~Channel()
{
	this->_clients.clear();
}

Channel &Channel::operator=(const Channel &rhs)
{
	this->_name = rhs.getName();
	this->_clients = rhs.getClients();
	this->_topic = rhs.getTopic();
	this->_limit = rhs.getLimit();
	this->_key = rhs.getKey();
	return (*this);
}

bool Channel::addMode(int mode)
{
	if (!(this->_chanmodes & mode))
	{
		this->_chanmodes = this->_chanmodes | mode;
		return (true);
	}
	return (false);
}

bool Channel::checkMode(int mode)
{
	return (this->_chanmodes & mode);
}

bool Channel::removeMode(int mode)
{
	if (this->_chanmodes & mode)
	{
		this->_chanmodes = this->_chanmodes & (~mode);
		return (true);
	}
	return (false);
}

std::pair<std::string, bool> Channel::changeModes(std::vector<std::string> params)
{
	params.erase(params.begin());
	std::string modestr = params.front();
	params.erase(params.begin());
	std::string changedmodes;
	bool invalidmode = false;
	size_t i = 0;
	while (i < modestr.size())
	{
		if (modestr[i] == '+')
		{
			changedmodes += "+";
			i++;
			while (i < modestr.size() && !strchr("+-", modestr[i]))
			{
				if (modestr[i] == 'o')
				{
					if (params.size() && this->_clientRights.count(params.front()))
					{
						this->addClientRight(this->_clients.at(params.front()), CHAN_OPERATOR);
						params.erase(params.begin());
						changedmodes += "o";
					}
				}
				else if (modestr[i] == 'b')
				{
					if (params.size())
					{
						this->addToBanList(params.front());
						params.erase(params.begin());
						changedmodes += "b";
					}
				}
				else if (modestr[i] == 'e')
				{
					if (params.size())
					{
						this->addToExceptList(params.front());
						params.erase(params.begin());
						changedmodes += "e";
					}
				}
				else if (modestr[i] == 'I')
				{
					if (params.size())
					{
						this->addToInviteList(params.front());
						params.erase(params.begin());
						changedmodes += "I";
					}
				}
				else if (modestr[i] == 'l')
				{
					if (params.size())
					{
						this->addMode(CHANMODE_LIMIT);
						this->setLimit(atoi(params.front().c_str()));
						changedmodes += "l";
					}
				}
				else if (modestr[i] == 'k')
				{
					if (params.size())
					{
						this->addMode(CHANMODE_KEY);
						this->setKey(params.front());
						changedmodes += "k";
					}
				}
				else if (modestr[i] == 'i' && this->addMode(CHANMODE_INVITE))
					changedmodes += "i";
				else if (modestr[i] == 'm' && this->addMode(CHANMODE_MOD))
					changedmodes += "m";
				else if (modestr[i] == 's' && this->addMode(CHANMODE_SECRET))
					changedmodes += "s";
				else if (modestr[i] == 't' && this->addMode(CHANMODE_TOPIC))
					changedmodes += "t";
				else if (modestr[i] == 'n' && this->addMode(CHANMODE_NOMSGFROMOUTSIDE))
					changedmodes += "n";
				else if (!strchr("beIlkimstn", modestr[i]))
					invalidmode = true;
				i++;
			}
			if (changedmodes.back() == '+')
				changedmodes.resize(changedmodes.size() - 1);
		}
		else if (modestr[i] == '-')
		{
			changedmodes += "-";
			i++;
			while (i < modestr.size() && !strchr("+-", modestr[i]))
			{
				if (modestr[i] == 'o')
				{
					if (params.size() && this->_clientRights.count(params.front()))
					{
						this->removeClientRight(this->_clients.at(params.front()), CHAN_OPERATOR);
						params.erase(params.begin());
						changedmodes += "o";
					}
				}
				else if (modestr[i] == 'b')
				{
					if (params.size())
					{
						this->removeFromBanList(params.front());
						params.erase(params.begin());
						changedmodes += "b";
					}
				}
				else if (modestr[i] == 'e')
				{
					if (!params.size())
					{
						this->removeFromExceptList(params.front());
						params.erase(params.begin());
						changedmodes += "e";
					}
				}
				else if (modestr[i] == 'I')
				{
					if (!params.size())
					{
						this->removeFromInviteList(params.front());
						params.erase(params.begin());
						changedmodes += "I";
					}
				}
				else if (modestr[i] == 'l' && this->removeMode(CHANMODE_LIMIT))
					changedmodes += "l";
				else if (modestr[i] == 'k' && this->removeMode(CHANMODE_KEY))
					changedmodes += "k";
				else if (modestr[i] == 'i' && this->removeMode(CHANMODE_INVITE))
					changedmodes += "i";
				else if (modestr[i] == 'm' && this->removeMode(CHANMODE_MOD))
					changedmodes += "m";
				else if (modestr[i] == 's' && this->removeMode(CHANMODE_SECRET))
					changedmodes += "s";
				else if (modestr[i] == 't' && this->removeMode(CHANMODE_TOPIC))
					changedmodes += "t";
				else if (modestr[i] == 'n' && this->removeMode(CHANMODE_NOMSGFROMOUTSIDE))
					changedmodes += "n";
				else if (!strchr("beIlkimstn", modestr[i]))
					invalidmode = true;
				i++;
			}
			if (changedmodes.back() == '-')
				changedmodes.resize(changedmodes.size() - 1);
		}
		else
			i++;
	}
	return (std::make_pair(changedmodes, invalidmode));
}

void Channel::addClientRight(Client *cl, int right)
{
	this->_clientRights.at(cl->getNickname()) = (this->_clientRights.at(cl->getNickname()) | right);
}

bool Channel::checkClientRight(Client *cl, int right)
{
	if (!this->_clientRights.count(cl->getNickname()))
		return (false);
	return (this->_clientRights.at(cl->getNickname()) & right);
}

void Channel::removeClientRight(Client *cl, int right)
{
	this->_clientRights.at(cl->getNickname()) = this->_clientRights.at(cl->getNickname()) & (~right);
}

void Channel::addClient(Client *cl)
{
	this->_clients.insert(std::make_pair(cl->getNickname(), cl));
	this->_clientRights.insert(std::make_pair(cl->getNickname(), 0));
}

size_t Channel::removeClient(Client *cl)
{
	this->_clientRights.erase(cl->getNickname());
	return(this->_clients.erase(cl->getNickname()));
}

size_t Channel::getSize() const
{
	return (this->_clients.size());
}

void Channel::setClient(std::string name, Client *client)
{
	this->_clients[name] = client;
}
Client *Channel::getClient(std::string name)
{
	if (!this->_clients.count(name))
		return (NULL);
	return this->_clients[name];
}
void Channel::setTopic(std::string s)
{
	this->_topic = s;
}
std::string Channel::getTopic() const
{
	return this->_topic;
}
void Channel::setLimit(int i)
{
	this->_limit = i;
}
size_t Channel::getLimit() const
{
	return this->_limit;
}
void Channel::setKey(std::string s)
{
	this->_key = s;
}
std::string Channel::getKey() const
{
	return this->_key;
}
void Channel::setClientRights(std::string s, int i)
{
	this->_clientRights[s] = i;
}
std::map<std::string, int> Channel::getClientRights() const
{
	return this->_clientRights;
}
std::string Channel::getName() const
{
	return this->_name;
}
void Channel::setName(std::string name)
{
	this->_name = name;
}
std::map<std::string, Client *> Channel::getClients() const
{
	return this->_clients;
}

std::string Channel::getNicklist()
{
	std::string list;
	for (std::map<std::string, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (this->checkClientRight(it->second, CHAN_OPERATOR))
			list += "@";
		list += it->second->getNickname() + " ";
	}
	list.resize(list.size() - 1);
	return (list);
}

void Channel::setClients(std::map<std::string, Client *> clients)
{
	this->_clients = clients;
}

void Channel::distributeMsg(std::string msg)
{
	for (std::map<std::string, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getSocket() > 0)
		{
			#if (DEBUG)
				std::cout << "send channel ===> " << it->second->getNickname() << "!" << this->getName() << " : " << msg;
			#endif
			if (it->second->getNickname() != "behaviourbot")
				send(it->second->getSocket(), msg.c_str(), msg.length(), 0);
		}
	}
}

bool Channel::ChannelHasClient(Client* cl)
{
	return(this->_clients.count(cl->getNickname()));
}

void Channel::addToBanList(std::string mask)
{
	this->_banlist.push_back(mask);
}

void Channel::removeFromBanList(std::string mask)
{

	this->_banlist.erase(std::find(this->_banlist.begin(), this->_banlist.end(), mask));
}

bool Channel::isBanned(std::string mask)
{
	for (std::vector<std::string>::iterator it = this->_banlist.begin(); it != this->_banlist.end(); it++)
	{
		if (matchMask(*it, mask))
			return (true);
	}
	return (false);
}

void Channel::addToExceptList(std::string mask)
{
	this->_exceptlist.push_back(mask);
}

void Channel::removeFromExceptList(std::string mask)
{
	this->_exceptlist.erase(std::find(this->_exceptlist.begin(), this->_banlist.end(), mask));
}

bool Channel::isOnExcept(std::string mask)
{
	for (std::vector<std::string>::iterator it = this->_exceptlist.begin(); it != this->_exceptlist.end(); it++)
	{
		if (matchMask(*it, mask))
		{
			return (true);
		}
	}
	return (false);
}

void Channel::addToInviteList(std::string mask)
{
	this->_invitelist.push_back(mask);
}

void Channel::removeFromInviteList(std::string mask)
{
	this->_invitelist.erase(std::find(this->_invitelist.begin(), this->_invitelist.end(), mask));
}

bool Channel::isOnInviteList(std::string mask)
{
	for (std::vector<std::string>::iterator it = this->_invitelist.begin(); it != this->_invitelist.end(); it++)
		if (!it->compare(mask))
			return (true);
	return (false);
}
