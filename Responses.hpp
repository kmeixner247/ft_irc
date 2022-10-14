#ifndef RESPONSES_HPP
#define RESPONSES_HPP
#define RPL_WELCOME				":<server> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>\r\n"
#define RPL_YOURHOST			":<server> 002 <nick> :Your host is besthost, running version <version>\r\n"
#define RPL_CREATED				":<server> 003 <nick> :This server was created <date>\r\n"
#define RPL_MYINFO				":<server> 004 <nick> :<servername> <version> <available user modes> <available channel modes>\r\n"
#define RPL_MOTDSTART 			":<server> 375 <nick> :- <server> Message of the day - \r\n"
#define RPL_MOTD				":<server> 372 <nick> :Kacper smells\r\n"
#define RPL_ENDOFMOTD			":<server> 376 <nick> :End of /MOTD command.\r\n"
//ERRORS
#define ERR_NOSUCHNICK			"401 <nick> :No such nick/channel"
#define ERR_NOSUCHSERVER		"402 <server> :No such server"
#define ERR_NOSUCHCHANNEL		"403 <channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN	"404 <chanenl name> :Cannot send to channel"
#define ERR_TOOMANYCHANNELS		"405 <channel name> :You have joined too many channels"
#define ERR_WASNOSUCHNICK		"406 <nickname> :There was no such nickname"
#define ERR_TOOMANYTARGETS		"407 <target> :Duplicate recipients. No message delivered"
#define ERR_NOORIGIN			"409 :no origin specified"
#define ERR_NORECIPIENT			"411 :No recipient given (<command>)"
#define ERR_NOTEXTTOSEND		"412 :No text to send"
#define ERR_NOTOPLEVEL			"413 :<mask> :No toplevel domain specified"
#define ERR_WILDTOPLEVEL		"414 :<mask> :Wildcard in toplevel domain"
#define UNKNOWNCOMMAND			"421 :<command> :Unknown command"
#define ERR_NOMOTD				"422 :MOTD File is missing"
#define ERR_NOADMININFO			"423 <server> :No administrative info available"
#define ERR_FILEERROR			"424 :File error doing <file op> on <file>"
#define ERR_NONICKNAMEGIVEN		"431 :No nickname given"
#define ERR_ERRONEUSNICKNAME	"432 :Erroneus nickname"
#define ERR_NICKNAMEINUSE		"433 <nick> :Nickname is already in use"
#define ERR_NICKCOLLISION		"436 <nick> :Nickname collision KILL"
#define ERR_USERNOTINCHANNEL	"441 <nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL		"442 <channel> :You're not on that channel"
#define ERR_USERONCHANNEL		"443 <user> <channel> :is already on channel"
#define ERR_NOLOGIN				"444 <user> :User not logged in"
#define ERR_SUMMONDISABLED		"445 :Summon has been disabled"
#define ERR_USERSDISABLED		"446 :USERS has been disabled"
#define ERR_NOTREGISTERED		"451 :You have not registered"
#define ERR_NEEDMOREPARAMS		"461 <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED	"462 :You may not reregister"
#define ERR_NOPERMFORHOST		"463 :Your host isn't among the privileged"
#define ERR_PASSWDMISMATCH		"464 :Password incorrect"
#define ERR_YOUREBANNEDCREEP	"465 :You are banned from this server"
#define ERR_KEYSET				"467 <channel> :Channel key already set"
#define ERR_CHANNELISFULL		"471 <channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE			"472 <char> :is unknown mode char to me"
#define ERR_INVITEONLYCHAN		"473 <channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN		"474 <channel> :Cannot join channel (+b)"
#define ERR_NOPRIVILIGES		"481 :Permission Denied- You're not an IRC operator"
#define ERR_CHANOPRIVSNEEDED	"482 <channel> :You're not channel operator"
#define ERR_CANTKILLSERVER		"483 :You cant kill a server!"
#define ERR_NOOPERHOST			"491 :No O-lines for your host"
#define ERR_UMODEUNKNOWNFLAG	"501 :Unknown MODE flag"
#define ERR_USERSDONTMATCH		"502 :Can't change mode for other users"


#endif
