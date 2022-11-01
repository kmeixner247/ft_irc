#include "Client.hpp"

Client::Client() : 
_socket(-1), 
_nickname(""), 
_username(""), 
_realname(""),
_usermodes(0),
_channel(std::map<std::string, Channel *>()),
_buffer("")
{
}

Client::Client(const Client &rhs)
{
	*this = rhs;
}

Client::Client(int socket) : 
_socket(socket), 
_nickname(""), 
_username(""), 
_realname(""), 
_usermodes(0),
_channel(std::map<std::string, Channel *>()), 
_buffer("")
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
	this->_usermodes = rhs._usermodes;
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

bool Client::addMode(int mode)
{
	if (!(this->_usermodes & mode))
	{
		this->_usermodes = this->_usermodes | mode;
		return (true);
	}
	return (false);
}

bool Client::checkMode(int mode)
{
	return (this->_usermodes & mode);
}

bool Client::removeMode(int mode)
{
	if (this->_usermodes & mode)
	{
		this->_usermodes = this->_usermodes & (~mode);
		return (true);
	}
	return (false);
}

std::pair<std::string, bool> Client::changeModes(std::string modestr)
{
	bool invalidmode = false;
	std::string changedmodes;
	size_t i = 0;
	while (i < modestr.size())
	{
		if (modestr[i] == '+')
		{
			changedmodes += "+";
			i++;
			while (i < modestr.size() && !strchr("+-", modestr[i]))
			{
				if (modestr[i] == 'i' && this->addMode(USERMODE_INVIS))
					changedmodes += "i";
				else if (modestr[i] == 's' && this->addMode(USERMODE_SERVERNOTICE))
					changedmodes += "s";
				else if (modestr[i] == 'w' && this->addMode(USERMODE_WALLOPRECEIVER))
					changedmodes += "w";
				else if (!strchr("isw", modestr[i]))
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
			while (i < modestr.size() && modestr[i] != '+' && modestr[i] != '-')
			{
				if (modestr[i] == 'i' && this->removeMode(USERMODE_INVIS))
					changedmodes += "i";
				else if (modestr[i] == 's' && this->removeMode(USERMODE_SERVERNOTICE))
					changedmodes += "s";
				else if (modestr[i] == 'w' && this->removeMode(USERMODE_WALLOPRECEIVER))
					changedmodes += "w";
				else if (modestr[i] == 'o' && this->removeMode(USERMODE_OP))
					changedmodes += "o";
				else if (!strchr("iswo", modestr[i]))
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

int Client::getSocket()
{
	return (this->_socket);
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
std::map<std::string, Channel*> Client::getChannels()
{
	return (this->_channel);
}
void Client::setSocket(int socket)
{
	this->_socket = socket;
}

void Client::setBuffer(std::string buffer)
{
	this->_buffer = buffer;
}

void Client::setAwayMsg(std::string msg)
{
	this->_awaymsg = msg;
}

std::string Client::getAwayMsg()
{
	return (this->_awaymsg);
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