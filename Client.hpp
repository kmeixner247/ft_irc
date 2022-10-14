#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
class Client
{
private:
	int _socket;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	bool _operator;
	bool _passbool;
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
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setRealname(std::string realname);
	void setOperator(bool isop);
	void setSocket(int socket);
	void setPassbool(bool b);
};
#endif