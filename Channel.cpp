#include "Channel.hpp"

Channel::Channel() : 
	_name(""), 
	_privateChan(false),  
	_secretChan(false), 
	_inviteOnly(false), 
	_topic(""), 
	_noMsgFromOutside(false), 
	_moderatedChan(false), 
	_limit(99999), 
	_key("")
{
}
Channel::Channel(const std::string &name) : 
	_name(name), 
	_privateChan(false), 
	_secretChan(false), 
	_inviteOnly(false), 
	_topic(""), 
	_noMsgFromOutside(false), 
	_moderatedChan(false), 
	_limit(99999), 
	_key("")
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
	this->_privateChan = rhs.getPrivateChan();
	this->_secretChan = rhs.getSecretChan();
	this->_clientRights = rhs.getClientRights();
	this->_inviteOnly = rhs.getInviteOnly();
	this->_topic = rhs.getTopic();
	this->_noMsgFromOutside = rhs.getNoMsgFromOutside();
	this->_moderatedChan = rhs.getModeratedChan();
	this->_limit = rhs.getLimit();
	// this->_banMask = rhs._banMask;
	this->_key = rhs.getKey();
	return (*this);
}

void Channel::addClientRight(Client *cl, int right)
{
	this->_clientRights.at(cl->getNickname()) = (this->_clientRights.at(cl->getNickname()) | right);
}

bool Channel::checkClientRight(Client *cl, int right)
{
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
	return this->_clients[name];
}
void Channel::setPrivateChan(bool b)
{
	this->_privateChan = b;
}
bool Channel::getPrivateChan() const
{
	return this->_privateChan;
}
void Channel::setSecretChan(bool b)
{
	this->_secretChan = b;
}
bool Channel::getSecretChan() const
{
	return this->_secretChan;
}
void Channel::setInviteOnly(bool b)
{
	this->_inviteOnly = b;
}
bool Channel::getInviteOnly() const
{
	return this->_inviteOnly;
}
void Channel::setTopic(std::string s)
{
	this->_topic = s;
}
std::string Channel::getTopic() const
{
	return this->_topic;
}
void Channel::setNoMsgFromOutside(bool b)
{
	this->_noMsgFromOutside = b;
}
bool Channel::getNoMsgFromOutside() const
{
	return this->_noMsgFromOutside;
}
void Channel::setModeratedChan(bool b)
{
	this->_moderatedChan = b;
}
bool Channel::getModeratedChan() const
{
	return this->_moderatedChan;
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

std::map<std::string, Client *> Channel::getClients() const
{
	return this->_clients;
}

void Channel::setClients(std::map<std::string, Client *> clients)
{
	this->_clients = clients;
}
