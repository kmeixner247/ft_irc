#include "Server.hpp"
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>
#include <cstring>

Server::Server() : 
_port(0), 
_password(""), 
_serverfd(0),
_host(""), 
_servername(""), 
_version(""), 
_motd(""),
_passwordOper("")
{
}

Server &Server::operator=(const Server &rhs)
{
	this->_port = rhs.getPort();
	this->_password = rhs.getPassword();
	this->_serverfd = rhs.getServerfd();
	this->_host = rhs.getHost();
	this->_servername = rhs.getServerName();
	this->_version = rhs.getVersion();
	this->_motd = rhs.getMotd();
	this->_passwordOper = rhs.getPasswordOper();
	return (*this);
}

Server::Server(const Server &rhs)
{
	*this = rhs;
}

Server::Server(int port, std::string pw) : 
_port(port), 
_password(pw), 
_serverfd(0),
_host("127.0.0.1"), 
_servername("awesomeserverofawesomeness"), 
_version("69.69"), 
_motd("kacper smells"),
_passwordOper("42069")
{
	if (this->init())
		throw "something went wrong in init";
}

Server::~Server()
{
	shutdown(this->_serverfd, SHUT_RDWR);
}

void Server::connectClient(int socket)
{
	FD_SET(socket, &this->_readfds);
	Client newclient(socket);
	this->_connectedclients[socket] = newclient;
	std::cout << "client connected" << std::endl;
}

void Server::disconnectClient(Client *cl)
{
	std::cout << "client disconnected" << std::endl;
	// FD_CLR(cl->getSocket(), &this->_readfds);

	this->_registeredclients.erase(cl->getNickname());
	if (!this->_connectedclients.erase(cl->getSocket()))
		throw "invalid socket disconnect";
}

void Server::serverloop()
{
	int addrlen = sizeof(this->_address);
	int newfd;
	int highest_socket;
	char buffer[1024] = {0};
	fd_set readfds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	std::string temp;
	while (42)
	{
		// checking whether there's a new client trying to connect
		// (make this a loop in case several try to connect at the same time?)
		if ((newfd = accept(this->_serverfd, (struct sockaddr *)&this->_address, (socklen_t *)&(addrlen))) > 0)
		{
			// making the client fd nonblocking (is this necessary?)
			if (fcntl(newfd, F_SETFL, O_NONBLOCK) == -1)
			{
				perror("fcntl failed");
				exit(EXIT_FAILURE);
			} 
			this->connectClient(newfd);
		}
		if (this->_connectedclients.size() > 0)
			highest_socket = this->_connectedclients.rbegin()->first;
		readfds = this->_readfds;
		if (highest_socket > 0)
		{
			//pls prettify oh god this is rly ugly
			if (select(highest_socket + 1, &readfds, NULL, NULL, &tv) > 0)
			{
				iterator tempend = this->_connectedclients.end();
				for (iterator it = this->_connectedclients.begin(); it != tempend; it++)
				{
					if (FD_ISSET(it->first, &readfds))
					{
						FD_CLR(it->first, &readfds);
						if (!recv(it->first, buffer, 1024, 0))
						{
							this->disconnectClient(&it->second);
						}
						else
							this->receiveMessage(&it->second, buffer);
						memset(buffer, 0, 1024);
						tempend = this->_connectedclients.end();
						it = this->_connectedclients.begin();
						if (_connectedclients.size() == 0)
						{
							memset(buffer, 0, 1024);
							break ;
						}
					}
				}
			}
		}
	}
}

int Server::init()
{
	int opt = 1;
	if ((this->_serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("server init: socket failed");
		return (-1);
	}
	if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("server init: setsockopt failed");
		return (-1);
	}
	if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("server init: setsockopt failed");
		return (-1);
	}
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
	if (bind(this->_serverfd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		perror("server init: bind failed");
		return (-1);
	}
	if (listen(this->_serverfd, 3) < 0)
	{
		perror("server init: listen failed");
		return (-1);
	}
	if (fcntl(this->_serverfd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("fcntl failed");
		return (-1);
	}
	return (0);
}

void Server::sendMsg(Client *cl, int argNum, std::string str, ...) const
{
	//prettify?
	std::string msg;
	va_list args;
	va_start(args, str);

	while (argNum-- != 0)
	{
		msg += str;
		if (argNum == 0)
			va_end(args);
		else
			str = va_arg(args, const char*);
	}
		std::cerr << "##############\nSENDING...\n" << msg << "...to" << cl->getNickname()<<"\n##############" << std::endl;
	send(cl->getSocket(), msg.c_str(), msg.length(), 0);
}

void Server::sendMsg(Channel *ch, int argNum, std::string str, ...) const
{
	//prettify?
	std::string msg;
	va_list args;
	va_start(args, str);

	while (argNum-- != 0)
	{
		msg += str;
		if (argNum == 0)
			va_end(args);
		else
			str = va_arg(args, const char*);
	}
	ch->distributeMsg(msg);
}

std::vector<Message> Server::parseMessages(Client *cl, std::string input)
{
	std::vector<Message> msgs;
	size_t pos;
	while ((pos = input.find('\n')) != input.npos)
	{
		if (input[pos - 1] == '\r')
			msgs.push_back(Message(input.substr(0, pos - 1)));
		else
			msgs.push_back(Message(input.substr(0, pos)));
		input = input.substr(pos + 1, input.npos);
	}
	cl->setBuffer(input);
	return (msgs);
}

std::string Server::makeClientPrefix(Client *cl)
{
	std::string prefix;
	prefix += ":" + cl->getNickname() + "!" + cl->getUsername() + "@" + this->_host;
	return (prefix);
}

void Server::removeClientFromChannel(Client *cl, Channel *ch)
{
	cl->removeChannel(ch);
	ch->removeClient(cl);
	if (ch->getClients().size() == 0)
		this->_channels.erase(ch->getName());
}

// void Server::interpretMessages(Client *cl, char *buffer)
void Server::interpretMessages(Client *cl, std::vector<Message> msgs)
{
	// std::vector<Message> msgs = parseMessages(buffer);
	for (std::vector<Message>::iterator it = msgs.begin(); it != msgs.end(); it++)
	{
		//errors?
		std::string command = it->getCommand();
		if (!(command.compare("PASS")))
			this->PASS(cl, *it);
		else// if (this->clientIsRegistered(cl))
		{
			if (!cl->getPassbool())
				return ;
			if (!command.compare("USER")) this->USER(cl, *it);
			else if (!command.compare("NICK")) this->NICK(cl, *it);
			else if (!(command.compare("QUIT"))) this->QUIT(cl, *it);
			else if (!(command.compare("JOIN"))) this->JOIN(cl, *it);
			else if (!(command.compare("KILL"))) this->KILL(cl, *it);
			else if (!(command.compare("OPER"))) this->OPER(cl, *it);
			else if (!(command.compare("SQUIT"))) this->SQUIT(cl, *it);
			else if (!(command.compare("PRIVMSG"))) this->PRIVMSG(cl, *it);
			else if (!(command.compare("WALLOPS"))) this->WALLOPS(cl, *it);
			else if (!(command.compare("NOTICE"))) this->NOTICE(cl, *it);
			else if (!(command.compare("KICK"))) this->KICK(cl, *it);
			else if (!(command.compare("MODE"))) this->MODE(cl, *it);
			else if (!(command.compare("INVITE"))) this->INVITE(cl, *it);
			else if (!(command.compare("TOPIC"))) this->TOPIC(cl, *it);
			else if (!(command.compare("WHO"))) this->WHO(cl, *it);
			else if (!(command.compare("PART"))) this->PART(cl, *it);
			else std::cout << "NONE OF THOSE1\n" << *it << std::endl;
		}
/* else if(!(this->clientIsRegistered(cl)))
		{
			if (!cl->getPassbool())
				return ;
			if (!command.compare("USER")) this->USER(cl, *it);
			else if (!command.compare("NICK")) this->NICK(cl, *it);
			else std::cout << "NONE OF THOSE2\n" << *it << std::endl;
		} */
	}
}

void Server::receiveMessage(Client *cl, char *buffer)
{
	cl->catBuffer(buffer);
	if (cl->getBuffer().find_first_of('\n') != std::string::npos)
		this->interpretMessages(cl, this->parseMessages(cl, cl->getBuffer()));
}

std::string msgMaker(int argNum, std::string str, ...)
{
    va_list args;
    va_start(args, str);
    std::string msgOutput;
    
    while (argNum-- != 0)
    {
        msgOutput += str;
        if (argNum == 0)
            va_end(args);
        else
            str = va_arg(args, const char*);
    }
    return msgOutput;
}

bool Server::clientIsConnected(Client *cl)
{
	return !(this->_connectedclients.find(cl->getSocket()) == _connectedclients.end());
}

bool Server::clientIsRegistered(Client *cl)
{
	return !(this->_registeredclients.find(cl->getNickname()) == _registeredclients.end());
}
void Server::setPort(int port)
{
	this->_port = port;
}
void Server::setRegisteredClients(std::map<std::string, Client *> clients)
{
	this->_registeredclients = clients;
}
std::map<std::string, Client *> Server::getRegisteredClients()
{
	return this->_registeredclients;
}
void Server::setConnectedClients(std::map<int, Client> clients)
{
	this->_connectedclients = clients;
}
std::map<int, Client> Server::getConnectedClients()
{
	return this->_connectedclients;
}
void Server::setHost(std::string host)
{
	this->_host = host;
}
std::string Server::getHost() const
{
	return this->_host;
}
void Server::setServerName(std::string servername)
{
	this->_servername = servername;
}
std::string Server::getServerName() const
{
	return this->_servername;
}
void Server::setVersion(std::string version)
{
	this->_version = version;
}
std::string Server::getVersion() const
{
	return this->_version;
}
void Server::setMotd(std::string motd)
{
	this->_motd = motd;
}
std::string Server::getMotd() const
{
	return this->_motd;
}
void Server::setChannels(std::map<std::string, Channel> channels)
{
	this->_channels = channels;
}
std::map<std::string, Channel> Server::getChannels() const
{
	return this->_channels;
}
Channel *Server::addChannel(Channel ch)
{
	std::map<std::string, Channel>::iterator it = this->_channels.insert(std::make_pair(ch.getName(), ch)).first;
	// return (this->_channels.insert(std::make_pair(ch.getName(), ch)));
	return (&it->second);
}
void Server::setPasswordOper(std::string password)
{
	this->_passwordOper = password;
}
std::string Server::getPasswordOper() const
{
	return this->_passwordOper;
}
std::string Server::getPassword() const
{
	return this->_password; 
}

int Server::getServerfd() const
{
	return this->_serverfd;
}

void Server::setServerfd(int serverfd)
{
	this->_serverfd = serverfd;
}

int Server::getPort() const
{
	return this->_port;
}

void Server::setPassword(std::string password)
{
	this->_password = password;
}