#include "Server.hpp"

std::string Server::ERR_NOSUCHNICK(Client *cl, std::string nick)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 401 ";
	msg += cl->getNickname() + " ";
	msg += nick;
	msg += " :No such nick/channel";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NOSUCHSERVER(Client *cl, std::string server)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 402 ";
	msg += cl->getNickname() + " ";
	msg += server;
	msg += " :No such server";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NOSUCHCHANNEL(Client *cl, std::string channel)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 403 ";
	msg += cl->getNickname() + " ";
	msg += channel;
	msg += " :No such channel";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_TOOMANYTARGETS(Client *cl, std::string target)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 407 ";
	msg += cl->getNickname() + " ";
	msg += target;
	msg += " :Duplicate recipients. No message delivered";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NORECIPIENT(Client *cl, std::string command)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 411 ";
	msg += cl->getNickname() + " ";
	msg += ":No recipient given (" + command + ")";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NOTEXTTOSEND(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 412 ";
	msg += cl->getNickname() + " ";
	msg += ":No text to send";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NOTOPLEVEL(Client *cl, std::string mask)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 413 ";
	msg += cl->getNickname() + " ";
	msg += mask;
	msg += " :No toplevel domain specified";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_WILDTOPLEVEL(Client *cl, std::string mask)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 414 ";
	msg += cl->getNickname() + " ";
	msg += mask;
	msg += " :Wildcard in toplevel domain";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NONICKNAMEGIVEN(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 431 ";
	msg += cl->getNickname();
	msg += " :No nickname";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_ERRONEUSNICKNAME(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 432 ";
	msg += cl->getNickname();
	msg += " :Erroneus nickname";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NICKNAMEINUSE(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 433 ";
	msg += cl->getNickname() + " " + cl->getNickname();
	msg += " :Nickname is already in use";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NOTONCHANNEL(Client *cl, std::string channel)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 442 ";
	msg += cl->getNickname() + " ";
	msg += channel;
	msg += " :You're not on that channel";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_NEEDMOREPARAMS(Client *cl, std::string command)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 461 ";
	msg += cl->getNickname() + " ";
	msg += command;
	msg += " :Not enough parameters";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_ALREADYREGISTERED(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 462 ";
	msg += cl->getNickname();
	msg += " :You may not reregister";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_CHANNELISFULL(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 471 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName() + " ";
	msg += " :Cannot join channel (+l)";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_INVITEONLYCHAN(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 473 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName() + " ";
	msg += " :Cannot join channel (+i)";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_BANNEDFROMCHAN(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 474 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName() + " ";
	msg += " :Cannot join channel (+b)";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_BADCHANNELKEY(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 475 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :Cannot join channel (+k)";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_PASSWDMISMATCH(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 464 ";
	msg += cl->getNickname();
	msg += " :Password incorrect";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_USERSDONTMATCH(Client *cl, std::string nick)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 502 ";
	msg += cl->getNickname() + " ";
	msg += nick;
	msg += " :Can't change mode for other users";
	msg += "\r\n";
	return (msg);
	//":<server> 502 <nick> :Can't change mode for other users\r\n"
}
