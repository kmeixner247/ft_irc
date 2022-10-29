#include "Client.hpp"

Client::Client() : _socket(-1), _nickname(""), _username(""), _realname(""), _operator(false), _passbool(false), _invisible(false), _serverNotice(false), _wallopReceiver(false), _buffer("")
{
}

Client::Client(const Client &rhs)
{
	*this = rhs;
}

Client::Client(int socket) : _socket(socket), _nickname(""), _username(""), _realname(""), _operator(false), _passbool(false), _invisible(false), _serverNotice(false), _wallopReceiver(false), _buffer("")
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
	this->_invisible = rhs._invisible;
	this->_serverNotice = rhs._serverNotice;
	this->_wallopReceiver = rhs._wallopReceiver;
	this->_buffer = rhs._buffer;
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
bool Client::getInvisible()
{
	return (this->_invisible);
}
bool Client::getServerNotice()
{
	return (this->_serverNotice);
}
bool Client::getWallopReceiver()
{
	return (this->_wallopReceiver);
}
std::string Client::getBuffer()
{
	return (this->_buffer);
}
void Client::catBuffer(char *str)
{
	this->_buffer += str;
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
void Client::setBuffer(std::string buffer)
{
	this->_buffer = buffer;
}
void Client::addChannel(Channel *ch)
{
	this->_channel.insert(std::make_pair(ch->getName(), ch));
}

size_t Client::removeChannel(Channel *ch)
{
	return(this->_channel.erase(ch->getName()));
}

bool Client::ClientIsInChannel(Channel* ch)
{
	return(this->_channel.count(ch->getName()));
}