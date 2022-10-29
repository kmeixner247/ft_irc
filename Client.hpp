#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include <map>
#include <vector>
#include "Channel.hpp"
#include <utility>
#define USERMODE_OP 1
#define USERMODE_PASS 2
#define USERMODE_INVIS 4
#define USERMODE_SERVERNOTICE 8
#define USERMODE_WALLOPRECEIVER 16
class Channel;
class Client
{
private:
	int _socket;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	int _usermodes;
	std::map<std::string, Channel *> _channel;
	std::string _buffer;

public:
	Client();
	Client(const Client &rhs);
	Client(int socket);
	~Client();
	Client &operator=(const Client &rhs);
	std::string getNickname();
	std::string getUsername();
	std::string getRealname();
	void addMode(int mode);
	bool checkMode(int mode);
	void removeMode(int mode);
	int getSocket();
	std::string getBuffer();
	void catBuffer(char *str);
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setSocket(int socket);
	void setBuffer(std::string buffer);
	void addChannel(Channel *ch);
	size_t removeChannel(Channel *ch);
	bool ClientIsInChannel(Channel *ch);
};
#endif