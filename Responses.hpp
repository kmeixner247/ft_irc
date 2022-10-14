#ifndef RESPONSES_HPP
#define RESPONSES_HPP
#define RPL_WELCOME		":<server> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>\r\n"
#define RPL_YOURHOST	":<server> 002 <nick> :Your host is besthost, running version <version>\r\n"
#define RPL_CREATED		":<server> 003 <nick> :This server was created <date>\r\n"
#define RPL_MYINFO		":<server> 004 <nick> :<servername> <version> <available user modes> <available channel modes>\r\n"
#define RPL_MOTDSTART 	":<server> 375 <nick> :- <server> Message of the day - \r\n"
#define RPL_MOTD		":<server> 372 <nick> :Kacper smells\r\n"
#define RPL_ENDOFMOTD	":<server> 376 <nick> :End of /MOTD command.\r\n"
#endif
