#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include "Client.hpp"

#define CHAN_MODERATOR 1
#define CHAN_OPERATOR 2
#define CHAN_BAN 4
#define CHAN_INVITE 8

class Client;

class Channel
{
private:
	const std::string _name;
	std::map<std::string, std::vector<Client*> > _clients;
	bool _privateChan;
	bool _secretChan;
	bool _inviteOnly;
	std::string _topic;
	bool _noMsgFromOutside;
	bool _moderatedChan;
	int _limit;
	//bool _banMask;
	std::string _key;
	std::map<std::string, int> _clientRights;

public:
	Channel();
	Channel(const Channel &rhs);
	~Channel();
	Channel &operator=(const Channel &rhs);
};

#endif