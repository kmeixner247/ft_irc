#include "Server.hpp"
std::string Server::JOINREPLY(Client *cl, Channel *ch)
{
	// this->sendResponse(cl, ch, ":<nick> JOIN #test\r\n");
	std::string msg;
	msg += ":" + cl->getNickname();
	msg += " JOIN ";
	msg += ch->getName();
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_TOPIC(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 332 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :" + ch->getTopic();
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_NAMREPLY(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 353 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :" + ch->getNicklist();
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_ENDOFNAMES(Client *cl, Channel *ch)
{
//":<server> 366 <nick> #test :End of /NAMES list\r\n"			  //??
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 366 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :End of /NAMES list";
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_MOTDSTART(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 375 ";
	msg += cl->getNickname() + " ";
	msg += ":- " + this->getServerName() + " Message of the day - ";
	msg += "\r\n";
	return (msg);
}
std::string Server::RPL_MOTD(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 372 ";
	msg += cl->getNickname() + " ";
	msg += ":" + this->getMotd();
	msg += "\r\n";
	return (msg);
}
std::string Server::RPL_ENDOFMOTD(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 376 ";
	msg += cl->getNickname() + " ";
	msg += ":End of /MOTD command.";
	msg += "\r\n";
	return (msg);
}
	// this->sendResponse(cl, RPL_MOTDSTART);
	// this->sendResponse(cl, RPL_MOTD);
	// this->sendResponse(cl, RPL_ENDOFMOTD);