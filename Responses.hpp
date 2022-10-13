#ifndef RESPONSES_HPP
#define RESPONSES_HPP
#define RPL_WELCOME		":myawesomeserver 001 gui :Welcome to the Internet Relay Network <nick>!<user>@<host>\r\n"
#define RPL_YOURHOST	":myawesomeserver 002 gui :Your host is besthost, running version <version>\r\n"
#define RPL_CREATED		":myawesomeserver 003 gui :This server was created <date>\r\n"
#define RPL_MYINFO		":myawesomeserver 004 gui :<servername> <version> <available user modes> <available channel modes>\r\n"
#define RPL_MOTDSTART 	":myawesomeserver 375 gui :- <server> Message of the day - \r\n"
#define RPL_MOTD		":myawesomeserver 372 gui :Kacper smells\r\n"
#define RPL_ENDOFMOTD	":myawesomeserver 376 gui :End of /MOTD command.\r\n"
#endif