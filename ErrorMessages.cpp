#include "Server.hpp"

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

std::string Server::ERR_BADCHANNELKEY(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 475 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName() + " ";
	msg += " :Cannot join channel (+k)";
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

