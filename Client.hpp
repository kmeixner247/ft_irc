#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include <map>
#include <vector>
#include "Channel.hpp"
#include <utility>
#define USERMODE_PASS 1
#define USERMODE_INVIS 2
#define USERMODE_SERVERNOTICE 4
#define USERMODE_WALLOPRECEIVER 8
#define USERMODE_OP 16
#define USERMODE_AWAY 32

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
	std::string _awaymsg;

public:
	Client();
	Client(const Client &rhs);
	Client(int socket);
	~Client();
	Client &operator=(const Client &rhs);
	std::string getNickname();
	std::string getUsername();
	std::string getRealname();
	std::map<std::string, Channel*> getChannels();
	bool addMode(int mode);
	bool checkMode(int mode);
	bool removeMode(int mode);
	std::pair<std::string, bool> changeModes(std::string modestr);
	int getSocket();
	std::string getBuffer();
	void catBuffer(char *str);
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setSocket(int socket);
	void setBuffer(std::string buffer);
	void setAwayMsg(std::string msg);
	std::string getAwayMsg();
	void addChannel(Channel *ch);
	size_t removeChannel(Channel *ch);
	bool ClientIsInChannel(Channel *ch);

};
#endif