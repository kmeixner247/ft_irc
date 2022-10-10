#include "Channel.hpp"

Channel::Channel() : _name("")
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