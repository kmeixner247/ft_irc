#pragma once
#ifndef BEHAVIOURBOT_HPP
#define BEHAVIOURBOT_HPP
#include <set>
#include "Client.hpp"
class Client;

class BehaviourBot
{
public:
	BehaviourBot();
	BehaviourBot(Client *cl);
	~BehaviourBot();
	bool checkBehaviour(std::string msg);
	bool addGoodWord(std::string word);
	bool addBadWord(std::string word);
	bool removeGoodWord(std::string word);
	bool removeBadWord(std::string word);
	Client *getBotClient();
	std::string getGoodWords();
	std::string getBadWords();
private:
	BehaviourBot &operator=(BehaviourBot &rhs);
	BehaviourBot(BehaviourBot &rhs);
	Client *_botclient;
	std::set<std::string> _goodwords;
	std::set<std::string> _badwords;
};
#endif