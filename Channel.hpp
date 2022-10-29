#pragma once
#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <map>
#include "Client.hpp"
#include <string>
#include <utility>
#define CHANMODE_BAN 1
#define CHANMODE_EXCEPT 2
#define CHANMODE_LIMIT 4
#define CHANMODE_INVITE 8
#define CHANMODE_INVITEEXCEPT 16
#define CHANMODE_KEY 32
#define CHANMODE_MOD 64
#define CHANMODE_SECRET 128
#define CHANMODE_TOPIC 256
#define CHANMODE_NOMSGFROMOUTSIDE 512
#define CHAN_OPERATOR 1
#define CHAN_MODERATOR 2
#define CHAN_BAN 4
#define CHAN_INVITE 8
#include <sys/socket.h>

class Client;

class Channel
{
private:
	std::string _name;
	std::map<std::string, Client*> _clients;
	int _chanmodes;
	std::string _topic;
	int _limit;
	std::string _key;
	std::map<std::string, int> _clientRights;

public:
	Channel();
	Channel(const Channel &rhs);
	Channel(const std::string &name);
	~Channel();
	Channel &operator=(const Channel &rhs);

	void addClientRight(Client *cl, int right);
	bool checkClientRight(Client *cl, int right);
	void removeClientRight(Client *cl, int right);
	void addMode(int mode);
	bool checkMode(int mode);
	void removeMode(int mode); 
	size_t getSize() const;
	void addClient(Client *cl);
	size_t removeClient(Client *cl);
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
	size_t getLimit() const;
	void setKey(std::string);
	std::string getKey() const;
	void setClientRights(std::string, int);
	std::map<std::string, int> getClientRights() const;
	std::string getName() const;
	void setName(std::string);
	std::map<std::string, Client*> getClients() const;
	std::string getNicklist();
	void setClients(std::map<std::string, Client*> clients);
	void distributeMsg(std::string msg);
	bool ChannelHasClient(Client* cl);
};

#endif

