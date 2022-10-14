#include "Client.hpp"

Client::Client(): _socket(-1), _nickname(""), _username(""), _realname(""), _operator(false), _passbool(false)
{
}

Client::Client(const Client &rhs)
{
    *this = rhs;
}

Client::Client(int socket): _socket(socket),  _nickname(""), _username(""), _realname(""), _operator(false), _passbool(false)
{
}

Client::~Client()
{
}

Client &Client::operator=(const Client &rhs)
{
    this->_socket = rhs._socket;
    this->_nickname = rhs._nickname;
    this->_username = rhs._username;
    this->_realname = rhs._realname;
    this->_operator = rhs._operator;
    this->_passbool = rhs._passbool;
    return (*this);
}
std::string Client::getNickname()
{
    return (this->_nickname);
}

std::string Client::getUsername()
{
    return (this->_username);
}

std::string Client::getRealname()
{
    return (this->_realname);
}

bool Client::getOperator()
{
    return (this->_operator);
}
int Client::getSocket() 
{
    return (this->_socket);
}
bool Client::getPassbool()
{
    return (this->_passbool);
}
void Client::setNickname(std::string nickname)
{
    this->_nickname = nickname;
}
void Client::setUsername(std::string username)
{
    this->_username = username;
}
void Client::setRealname(std::string realname)
{
    this->_realname = realname;
}
void Client::setOperator(bool isop)
{
    this->_operator = isop;
}
void Client::setSocket(int socket)
{
    this->_socket = socket;
}
void Client::setPassbool(bool b)
{
    this->_passbool = b;
}