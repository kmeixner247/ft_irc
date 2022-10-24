#include "Channel.hpp"

Channel::Channel() : _name(""), _privateChan(false), _inviteOnly(false), _secretChan(false), _topic(""), _noMsgFromOutside(false), _moderatedChan(false), _limit(99999), /*_banMask(),*/ _key("");
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
	this->_name = rhs._name;
	this->_clients = rhs._clients;
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
bool Channel::getPrivateChan()
{
	return this->_privateChan;
}
void Channel::setSecretChan(bool b)
{
	this->_secretChan = b;
}
bool Channel::getSecretChan()
{
	return this->_secretChan;
}
void Channel::setInviteOnly(bool b)
{
	this->_inviteOnly = b;
}
bool Channel::getInviteOnly()
{
	return this->_inviteOnly;
}
void Channel::setTopic(std::string s)
{
	this->_topic = s;
}
std::string Channel::getTopic()
{
	return this->_topic;
}
void Channel::setNoMsgFromOutside(bool b)
{
	this->_noMsgFromOutside = b;
}
bool Channel::getNoMsgFromOutside()
{
	return this->_noMsgFromOutside;
}
void Channel::setModeratedChan(bool b)
{
	this->_moderatedChan = b;
}
bool Channel::getModeratedChan()
{
	return this->_moderatedChan;
}
void Channel::setLimit(int i)
{
	this->_limit = i;
}
int Channel::getLimit()
{
	return this->_limit;
}
void Channel::setKey(std::string s)
{
	this->_key = s;
}
std::string Channel::getKey()
{
	return this->_key;
}
void Channel::setClientRights(std::string s, int i)
{
	this->_clientRights[s] = i;
}
int Channel::getClientRights(std::string s)
{
	return this->_clientRights[s];
}
