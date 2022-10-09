#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
class Client
{
private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
public:
    Client();
    Client(const Client &rhs);
    ~Client();
    Client &operator=(const Client &rhs);
private:
};
#endif