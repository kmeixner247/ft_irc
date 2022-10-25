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
	std::map<std::string, Client*> _clients;
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
	Client *getClient(std::string name);
	void setClient(std::string name, Client *client);
	void setPrivateChan(bool);
	bool getPrivateChan();
	void setSecretChan(bool);
	bool getSecretChan();
	void setInviteOnly(bool);
	bool getInviteOnly();
	void setTopic(std::string);
	std::string getTopic();
	void setNoMsgFromOutside(bool);
	bool getNoMsgFromOutside();
	void setModeratedChan(bool);
	bool getModeratedChan();
	void setLimit(int);
	int getLimit();
	void setKey(std::string);
	std::string getKey();
	void setClientRights(std::string, int);
	int getClientRights(std::string);

};

#endif