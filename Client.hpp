#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include <map>
#include <vector>
#include "Channel.hpp"

class Client
{
private:
	int _socket;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	bool _operator;
	bool _passbool;
	bool _invisible;
	bool _serverNotice;
	bool _wallopReceiver;
	std::map<std::string, std::vector<Channel*> > _channel;
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
	bool getOperator();
	int getSocket();
	bool getPassbool();
	bool getInvisible();
	bool getServerNotice();
	bool getWallopReceiver();
	std::string getBuffer();
	void catBuffer(char *str);
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setOperator(bool isop);
	void setSocket(int socket);
	void setPassbool(bool b);
	void setBuffer(std::string buffer);
	void addChannel(Channel *ch);
};
#endif