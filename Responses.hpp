#pragma once
#ifndef RESPONSES_HPP
#define RESPONSES_HPP
// #define RPL_WELCOME ":<server> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>\r\n"
// #define RPL_YOURHOST ":<server> 002 <nick> :Your host is besthost, running version <version>\r\n"
// #define RPL_CREATED ":<server> 003 <nick> :This server was created <date>\r\n"
// #define RPL_MYINFO ":<server> 004 <nick> :<servername> <version> <available user modes> <available channel modes>\r\n"
// #define RPL_TOPIC ":<server> 332 <nick> <channel> :<topic>\r\n" //??
// #define RPL_NAMREPLY			":<server> 353 <nick> <channel> :<nicklist>" //??
// #define RPL_NAMREPLY ":<server> 353 <nick> #test :@bigboi smolboi otherboi @adminboi\r\n" //??
// #define RPL_ENDOFNAMES ":<server> 366 <nick> <channel> :End of /NAMES list\r\n"			  //??
// #define RPL_ENDOFNAMES ":<server> 366 <nick> #test :End of /NAMES list\r\n"			  //??
// #define RPL_MOTDSTART ":<server> 375 <nick> :- <server> Message of the day - \r\n"
// #define RPL_MOTD ":<server> 372 <nick> :Kacper smells\r\n"
// #define RPL_ENDOFMOTD ":<server> 376 <nick> :End of /MOTD command.\r\n"
// ERRORS

// #define ERR_NOSUCHNICK ":<server> 401 <nick> <nick> :No such nick/channel\r\n"
// #define ERR_NOSUCHSERVER ":<server> 402 <nick> <server> :No such server\r\n"
// #define ERR_NOSUCHCHANNEL ":<server> 403 <nick> <channel> :No such channel\r\n"
// #define ERR_CANNOTSENDTOCHAN ":<server> 404 <nick> <channel> :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS ":<server> 405 <nick> <channel> :You have joined too many channels\r\n"
#define ERR_WASNOSUCHNICK ":<server> 406 <nick> <nickname> :There was no such nickname\r\n"
// #define ERR_TOOMANYTARGETS ":<server> 407 <nick> <target> :Duplicate recipients. No message delivered\r\n"
#define ERR_NOORIGIN ":<server> 409 <nick> :no origin specified\r\n"
// #define ERR_NORECIPIENT ":<server> 411 <nick> :No recipient given (<command>)\r\n"
// #define ERR_NOTEXTTOSEND ":<server> 412 <nick> :No text to send\r\n"
// #define ERR_NOTOPLEVEL ":<server> 413 <nick> :<mask> :No toplevel domain specified\r\n"
// #define ERR_WILDTOPLEVEL ":<server> 414 <nick> :<mask> :Wildcard in toplevel domain\r\n"
#define UNKNOWNCOMMAND ":<server> 421 <nick> :<command> :Unknown command\r\n"
#define ERR_NOMOTD ":<server> 422 <nick> :MOTD File is missing\r\n"
#define ERR_NOADMININFO ":<server> 423 <nick> <server> :No administrative info available\r\n"
#define ERR_FILEERROR ":<server> 424 <nick> :File error doing <file op> on <file>\r\n"
// #define ERR_NONICKNAMEGIVEN ":<server> 431 <nick> :No nickname given\r\n"
// #define ERR_ERRONEUSNICKNAME ":<server> 432 <nick> :Erroneus nickname\r\n"
// #define ERR_NICKNAMEINUSE ":<server> 433 <nick> <nick> :Nickname is already in use\r\n"
#define ERR_NICKCOLLISION ":<server> 436 <nick> <nick> :Nickname collision KILL\r\n"
#define ERR_USERNOTINCHANNEL ":<server> 441 <nick> <nick> <channel> :They aren't on that channel\r\n"
// #define ERR_NOTONCHANNEL ":<server> 442 <nick> <channel> :You're not on that channel\r\n"
#define ERR_USERONCHANNEL ":<server> 443 <nick> <user> <channel> :is already on channel\r\n"
#define ERR_NOLOGIN ":<server> 444 <nick> <user> :User not logged in\r\n"
#define ERR_SUMMONDISABLED ":<server> 445 <nick> :Summon has been disabled\r\n"
#define ERR_USERSDISABLED ":<server> 446 <nick> :USERS has been disabled\r\n"
#define ERR_NOTREGISTERED ":<server> 451 <nick> :You have not registered\r\n"
// #define ERR_NEEDMOREPARAMS ":<server> 461 <nick> <command> :Not enough parameters\r\n"
// #define ERR_ALREADYREGISTRED ":<server> 462 <nick> :You may not reregister\r\n"
#define ERR_NOPERMFORHOST ":<server> 463 <nick> :Your host isn't among the privileged\r\n"
//#define ERR_PASSWDMISMATCH ":<server> 464 <nick> :Password incorrect\r\n"
#define ERR_YOUREBANNEDCREEP ":<server> 465 <nick> :You are banned from this server\r\n"
#define ERR_KEYSET ":<server> 467 <nick> <channel> :Channel key already set\r\n"
// #define ERR_CHANNELISFULL ":<server> 471 <nick> <channel> :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE ":<server> 472 <nick> <char> :is unknown mode char to me\r\n"
// #define ERR_INVITEONLYCHAN ":<server> 473 <nick> <channel> :Cannot join channel (+i)\r\n"
// #define ERR_BANNEDFROMCHAN ":<server> 474 <nick> <channel> :Cannot join channel (+b)\r\n"
// #define ERR_BADCHANNELKEY ":<server> 475 <nick> <channel> :Cannot join channel (+k)\r\n"
#define ERR_NOPRIVILIGES ":<server> 481 <nick> :Permission Denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED ":<server> 482 <nick> <channel> :You're not channel operator\r\n"
#define ERR_CANTKILLSERVER ":<server> 483 <nick> :You cant kill a server!\r\n"
#define ERR_NOOPERHOST ":<server> 491 <nick> :No O-lines for your host\r\n"
#define ERR_UMODEUNKNOWNFLAG ":<server> 501 <nick> :Unknown MODE flag\r\n"
// #define ERR_USERSDONTMATCH ":<server> 502 <nick> :Can't change mode for other users\r\n"
#endif
