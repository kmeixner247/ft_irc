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
	std::string _name;
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
	bool getPrivateChan() const;
	void setSecretChan(bool);
	bool getSecretChan() const;
	void setInviteOnly(bool);
	bool getInviteOnly() const;
	void setTopic(std::string);
	std::string getTopic() const;
	void setNoMsgFromOutside(bool);
	bool getNoMsgFromOutside() const;
	void setModeratedChan(bool);
	bool getModeratedChan() const;
	void setLimit(int);
	int getLimit() const;
	void setKey(std::string);
	std::string getKey() const;
	void setClientRights(std::string, int);
	std::map<std::string, int> getClientRights() const;
	std::string getName() const;
	std::map<std::string, Client*> getClients() const;
	void setClients(std::map<std::string, Client*> clients);
};

#endif