#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include <Client.hpp>
class Channel
{
private:
    std::string _name;
    std::map<std::string, Client> _clients;

public:
    Channel();
    Channel(const Channel &rhs);
    ~Channel();
    Channel &operator=(const Channel &rhs);
};

#endif