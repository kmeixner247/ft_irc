#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "Client.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "BehaviourBot.hpp"
class Client;
class Channel;
class BehaviourBot;
class Server
{
typedef std::map<int, Client>::iterator	iterator;
private:
	int _port;
	std::string _password;
	int _serverfd;
	struct sockaddr_in _address;
	std::map<std::string, Client*> _registeredclients;
	fd_set _readfds;
	std::map<int, Client> _connectedclients;
	std::string _host;
	std::string _servername;
	std::string _version;
	std::string _motd;
	std::string _passwordOper;
	std::map<std::string, Channel> _channels;
	std::map<std::string, Client> _bots;
	BehaviourBot *_karen;

public:
	static bool s_active;
	Server(int port, std::string pw, BehaviourBot *bot);
	~Server();
	void serverloop();
	void connectClient(int socket);
	void disconnectClient(Client *cl);
	void receiveMessage(Client *cl, char *buffer);
	void interpretMessages(Client *cl, std::vector<Message> msgs);
	void sendMsg(Client *cl, int argNum, std::string str, ...) const;
	void sendMsg(Channel *ch, int argNum, std::string str, ...) const;
	/* GETTERS AND SETTERS */
	void setPort(int port);
	void setPassword(std::string pw);
	void setRegisteredClients(std::map<std::string, Client*> clients);
	std::map<std::string, Client*> getRegisteredClients();
	void setConnectedClients(std::map<int, Client> clients);
	std::map<int, Client> getConnectedClients();
	void setHost(std::string host);
	std::string getHost() const;
	void setServerName(std::string name);
	std::string getServerName() const;
	void setVersion(std::string version);
	std::string getVersion() const;
	void setMotd(std::string motd);
	std::string getMotd() const;
	void setPasswordOper(std::string pw);
	std::string getPasswordOper() const;
	void setChannels(std::map<std::string, Channel> channels);
	std::map<std::string, Channel> getChannels() const;
	Channel *addChannel(Channel);
	std::string getPassword() const;
	int getPort() const;
	int getServerfd() const;
	void setServerfd(int fd);
private:
	void sigint_handler(int sig);
	std::string msgMaker(int argNum, std::string, ...);
	Server();
	Server(const Server &rhs);
	Server &operator=(const Server &rhs);
	int init();
	std::vector<Message> parseMessages(Client *cl, std::string input);
	void removeClientFromChannel(Client *cl, Channel *ch);
	/* COMMANDS */
	void AWAY(Client *cl, Message msg);
	void PASS(Client *cl, Message msg);
	void USER(Client *cl, Message msg);
	void NICK(Client *cl, Message msg);
	void JOIN(Client *cl, Message msg);
	void QUIT(Client *cl, Message msg);
	void KILL(Client *cl, Message msg);
	void PING(Client *cl, Message msg);
	void OPER(Client *cl, Message msg);
	void SQUIT(Client *cl, Message msg);
	void PRIVMSG(Client *cl, Message msg);
	void WALLOPS(Client *cl, Message msg);
	void NOTICE(Client *cl, Message msg);
	void KICK(Client *cl, Message msg);
	void MODE(Client *cl, Message msg);
	void INVITE(Client *cl, Message msg);
	void TOPIC(Client *cl, Message msg);
	void WHO(Client *cl, Message msg);
	void PART(Client *cl, Message msg);
	void DIE(Client *cl);
	void ADDBAD(Client *cl, Message msg);
	void RMBAD(Client *cl, Message msg);
	void ADDGOOD(Client *cl, Message msg);
	void RMGOOD(Client *cl, Message msg);
	void BHVLIST(Client *cl);
	/* CHANNEL DISTRIBUTION STUFF? */
	std::string JOINREPLY(Client *cl, Channel *ch);
	std::string PRIVMSGREPLY(Client *from, std::string to, std::string text);
	std::string NOTICEREPLY(Client *from, std::string to, std::string text);
	std::string QUITREPLY(Client *cl, std::string reason);
	std::string PARTREPLY(Client *cl, std::string channel, std::string reason);
	std::string MODEREPLY(Client *cl, std::string target, std::string modestr, std::vector<std::string> args);
	std::string MODEREPLY(Client *cl, std::string target, std::string modestr, std::string arg);
	std::string TOPICREPLY(Client *cl, Channel *ch, std::string topic);
	std::string KICKREPLY(Client *cl, Channel *ch, Client *target, std::string comment);
	std::string INVITEREPLY(Client *cl, Channel *ch, Client *target);
	std::string PONGREPLY(std::string token);
	std::string ERROR(Client *cl, std::string reason);
	std::string BOTREPLY(std::string to, std::string text);
	std::string BHVLISTREPLY(Client *cl, std::string text);
	/* RESPONSES */
	std::string RPL_UMODEIS(Client *cl);
	std::string RPL_ENDOFWHO(Client *cl, std::string mask);
	std::string RPL_AWAY(Client *cl, Client *toCl);
	std::string RPL_UNAWAY(Client *cl);
	std::string RPL_NOWAWAY(Client *cl);
	std::string RPL_CHANNELMODEIS(Client *cl, Channel *ch);
	std::string RPL_NOTOPIC(Client *cl, Channel *ch);
	std::string RPL_TOPIC(Client *cl, Channel *ch);
	std::string RPL_INVITELIST(Client *cl, Channel *ch);
	std::string RPL_ENDOFINVITELIST(Client *cl);
	std::string RPL_EXCEPTLIST(Client *cl, Channel *ch);
	std::string RPL_ENDOFEXCEPTLIST(Client *cl);
	std::string RPL_INVITING(Client *cl, std::string nick, std::string channel);
	std::string RPL_WHOREPLY(Client *cl, Client *target);
	std::string RPL_NAMREPLY(Client *cl, Channel *ch);
	std::string RPL_ENDOFNAMES(Client *cl, Channel *ch);
	std::string RPL_BANLIST(Client *cl, Channel *ch);
	std::string RPL_ENDOFBANLIST(Client *cl);
	std::string RPL_MOTD(Client *cl);
	std::string RPL_MOTDSTART(Client *cl);
	std::string RPL_ENDOFMOTD(Client *cl);
	std::string RPL_YOUREOPER(Client *cl);

	/* ERRORS */
	std::string ERR_ALREADYREGISTERED(Client *cl);
	std::string ERR_NEEDMOREPARAMS(Client *cl, std::string command);
	std::string ERR_NONICKNAMEGIVEN(Client *cl);
	std::string ERR_ERRONEUSNICKNAME(Client *cl);
	std::string ERR_NICKNAMEINUSE(Client *cl);
	std::string ERR_USERNOTINCHANNEL(Client *cl, std::string nick, std::string channel);
	std::string ERR_NOTONCHANNEL(Client *cl, std::string channel);
	std::string ERR_USERONCHANNEL(Client *cl, Channel *ch, std::string target);
	std::string ERR_NOSUCHCHANNEL(Client *cl, std::string channel);
	std::string ERR_BADCHANNELKEY(Client *cl, Channel *ch);
	std::string ERR_INVITEONLYCHAN(Client *cl, Channel *ch);
	std::string ERR_BANNEDFROMCHAN(Client *cl, Channel *ch);
	std::string ERR_CHANNELISFULL(Client *cl, Channel *ch);
	std::string ERR_NOSUCHNICK(Client *cl, std::string nick);
	std::string ERR_NOSUCHSERVER(Client *cl, std::string server);
	std::string ERR_TOOMANYTARGETS(Client *cl, std::string channel);
	std::string ERR_CANNOTSENDTOCHAN(Client *cl, std::string target);
	std::string ERR_NORECIPIENT(Client *cl, std::string command);
	std::string ERR_NOTEXTTOSEND(Client *cl);
	std::string ERR_NOTOPLEVEL(Client *cl, std::string mask);
	std::string ERR_WILDTOPLEVEL(Client *cl, std::string mask);
	std::string ERR_PASSWDMISMATCH(Client *cl);
	std::string ERR_NOPRIVILEGES(Client *cl);
	std::string ERR_CHANOPRIVSNEEDED(Client *cl, Channel *ch);
	std::string ERR_UMODEUNKNOWNFLAG(Client *cl);
	std::string ERR_USERSDONTMATCH(Client *cl, std::string nick);

	bool clientIsConnected(Client *cl);
	bool clientIsRegistered(Client *cl);
	void unexpectedQuit(Client *cl);

};
bool matchMask(std::string mask, std::string str);
std::string makeNickMask(Server *sv, Client *cl);
#endif
