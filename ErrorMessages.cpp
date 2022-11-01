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

std::string Server::ERR_CANNOTSENDTOCHAN(Client *cl, std::string target)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 404 ";
	msg += cl->getNickname() + " ";
	msg += target + " ";
	msg += ": Cannot send to channel";
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

std::string Server::ERR_USERNOTINCHANNEL(Client *cl, std::string nick, std::string channel)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 441 ";
	msg += cl->getNickname() + " ";
	msg += nick + " ";
	msg += channel + " ";
	msg += ":They aren't on that channel";
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
std::string Server::ERR_USERONCHANNEL(Client *cl, Channel *ch, std::string target) 
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 443 ";
	msg += cl->getNickname() + " ";
	msg += target + " ";
	msg += ch->getName() + " ";
	msg += ":is already on channel";
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

std::string Server::ERR_NOPRIVILEGES(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 481 ";
	msg += cl->getNickname() + " ";
	msg += ":Permission Denied- You're not an IRC operator";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_CHANOPRIVSNEEDED(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 482 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :You're not channel operator";
	msg += "\r\n";
	return (msg);
}

std::string Server::ERR_UMODEUNKNOWNFLAG(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 501 ";
	msg += cl->getNickname();
	msg += " :Unknown MODE flag";
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
}
