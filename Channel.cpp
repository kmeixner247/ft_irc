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