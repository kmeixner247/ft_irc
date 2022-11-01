#include "BehaviourBot.hpp"

BehaviourBot::BehaviourBot(Client *cl) :
_botclient(cl),
_goodwords(std::set<std::string>()),
_badwords(std::set<std::string>())
{
	this->_botclient->setNickname("behaviourbot");
	this->_botclient->setUsername("soccermom123");
	this->_botclient->setRealname("karen");
	this->_goodwords.insert("please");
	this->_badwords.insert("designmaggi");
}

BehaviourBot::~BehaviourBot()
{
}

bool BehaviourBot::checkBehaviour(std::string msg)
{
	for (std::set<std::string>::iterator it = this->_goodwords.begin(); it != this->_goodwords.end(); it++)
		if (msg.find(*it) == std::string::npos)
			return (true);
	for (std::set<std::string>::iterator it = this->_badwords.begin(); it != this->_badwords.end(); it++)
		if (msg.find(*it) != std::string::npos)
			return (true);
	return (false);
}

bool BehaviourBot::addGoodWord(std::string word)
{
	if (this->_badwords.count(word))
		return (false);
	return (this->_goodwords.insert(word).second);
}

bool BehaviourBot::removeGoodWord(std::string word)
{
	return (this->_goodwords.erase(word));
}

bool BehaviourBot::addBadWord(std::string word)
{
	if (this->_goodwords.count(word))
		return (false);
	return (this->_badwords.insert(word).second);
}

bool BehaviourBot::removeBadWord(std::string word)
{
	if (word != "designmaggi")
		return (this->_badwords.erase(word));
	return (false);
}

Client *BehaviourBot::getBotClient()
{
	return (this->_botclient);
}

std::string BehaviourBot::getGoodWords()
{
	std::string text;
	for (std::set<std::string>::iterator it = this->_goodwords.begin(); it != this->_goodwords.end(); it++)
		text += *it + " ";
	if (text.size())
		text.resize(text.size() - 1);
	text = "GOOD WORDS : " + text;
	return (text);
}

std::string BehaviourBot::getBadWords()
{
	std::string text;
	for (std::set<std::string>::iterator it = this->_badwords.begin(); it != this->_badwords.end(); it++)
		text += *it + " ";
	if (text.size())
		text.resize(text.size() - 1);
	text = "BAD WORDS : " + text;
	return (text);
}