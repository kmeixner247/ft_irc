#include "Server.hpp"
std::string Server::JOINREPLY(Client *cl, Channel *ch)
{
	// this->sendResponse(cl, ch, ":<nick> JOIN #test\r\n");
	std::string msg;
	msg +=  ":" +this->makeNickMask(cl);
	msg += " JOIN :";
	msg += ch->getName();
	msg += "\r\n";
	return (msg);
}

std::string Server::PRIVMSGREPLY(Client *from, std::string to, std::string text)
{
	std::string msg;
	msg += ":" + this->makeNickMask(from);
	msg += " PRIVMSG ";
	msg += to + " :";
	msg += text;
	msg += "\r\n";
	return (msg);
}

std::string Server::NOTICEREPLY(Client *from, std::string to, std::string text)
{
	std::string msg;
	msg += ":" + this->makeNickMask(from);
	msg += " NOTICE ";
	msg += to + " :";
	msg += text;
	msg += "\r\n";
	return (msg);
}

std::string Server::QUITREPLY(Client *cl, std::string reason)
{
	std::string msg;
	msg += ":" + this->makeNickMask(cl);
	msg += " QUIT ";
	msg += ":" + reason;
	msg += "\r\n";
	return (msg);
}

std::string Server::PARTREPLY(Client *cl, std::string channel, std::string reason)
{
	std::string msg;
	msg += ":" + this->makeNickMask(cl);
	msg += " PART ";
	msg += channel;
	if (reason != "")
		msg += " :" + reason;
	msg += "\r\n";
	return (msg);
}

std::string Server::MODEREPLY(Client *cl, std::string target, std::string modestr, std::vector<std::string> args)
{
	std::string msg;
	msg += ":" + this->makeNickMask(cl);
	msg += " MODE ";
	msg += target + " ";
	msg += modestr;
	std::vector<std::string>::iterator it = args.begin();
	it++;
	it++;
	for (; it != args.end(); it++)
		msg += " " + *it;
	msg += "\r\n";
	return (msg);
}

std::string Server::MODEREPLY(Client *cl, std::string target, std::string modestr, std::string arg)
{
	std::string msg;
	msg += ":" + this->makeNickMask(cl);
	msg += " MODE ";
	msg += target + " ";
	msg += modestr;
	msg += " " + arg;
	msg += "\r\n";
	return (msg);
}

std::string Server::TOPICREPLY(Client *cl, Channel *ch, std::string topic)
{
	std::string msg;
	msg += ":" + makeNickMask(cl);
	msg += " TOPIC ";
	msg += ch->getName() + " ";
	msg += topic;
	msg += "\r\n";
	return (msg);
}

std::string Server::KICKREPLY(Client *cl, Channel *ch, Client *target, std::string comment)
{
	std::string msg;
	msg += ":" + makeNickMask(cl);
	msg += " KICK ";
	msg += ch->getName() + " ";
	msg += target->getNickname() + " ";
	if (comment != "")
		msg += ":" + comment;
	msg += "\r\n";
	return (msg);
}

std::string Server::ERROR(Client *cl, std::string reason)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " ERROR ";
	msg += cl->getNickname() + " ";
	msg += reason;
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_UMODEIS(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 221 ";
	msg += cl->getNickname() + " +";
	if (cl->checkMode(USERMODE_INVIS))
		msg += "i";
	if (cl->checkMode(USERMODE_SERVERNOTICE))
		msg += "s";
	if (cl->checkMode(USERMODE_WALLOPRECEIVER))
		msg += "w";	
	if (cl->checkMode(USERMODE_OP))
		msg += "o";
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_CHANNELMODEIS(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 324 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName() + " +";
	if (ch->checkMode(CHANMODE_BAN))
		msg += "b";
	if (ch->checkMode(CHANMODE_EXCEPT))
		msg += "e";
	if (ch->checkMode(CHANMODE_LIMIT))
		msg += "l";
	if (ch->checkMode(CHANMODE_INVITE))
		msg += "i";
	if (ch->checkMode(CHANMODE_INVITEEXCEPT))
		msg += "I";
	if (ch->checkMode(CHANMODE_KEY))
		msg += "k";
	if (ch->checkMode(CHANMODE_MOD))
		msg += "m";
	if (ch->checkMode(CHANMODE_SECRET))
		msg += "s";
	if (ch->checkMode(CHANMODE_TOPIC))
		msg += "t";
	if (ch->checkMode(CHANMODE_NOMSGFROMOUTSIDE))
		msg += "n";
	msg += "\r\n";
	return (msg);
}

std::string Server::RPL_NOTOPIC(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 331 ";
	msg += cl->getNickname() + " ";
	msg += ch->getName();
	msg += " :No topic is set";
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
//RPL_INVITELIST 336
//RPL_ENDOFINVITELIST 337
//RPL_EXCEPTLIST 348
//RPL_ENDOFEXCEPTLIST 349

std::string Server::RPL_NAMREPLY(Client *cl, Channel *ch)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 353 ";
	msg += cl->getNickname();
	// if (ch->getPrivateChan())
	// 	msg += " * ";
	if (ch->checkMode(CHANMODE_SECRET))
		msg += " @ ";
	else
		msg += " = ";
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

//RPL_BANLIST 367
//RPL_ENDOFBANLIST 368



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

std::string Server::RPL_YOUREOPER(Client *cl)
{
	std::string msg;
	msg += ":" + this->getServerName();
	msg += " 381 ";
	msg += cl->getNickname() + " ";
	msg += ":You are now an IRC operator";
	msg += "\r\n";
	return (msg);
}