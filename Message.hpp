#pragma once
#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <vector>
class Message
{
private:
	std::string _prefix;
	std::string _command;
	std::vector<std::string> _parameters;
public:
	Message();
	Message(const Message &rhs);
	Message(const std::string &input);
	~Message();
	Message &operator=(const Message &rhs);
	void parse(std::string input);
	void clear();
	std::string getRaw() const;
	std::string getPrefix() const {return this->_prefix;}
	std::string getCommand() const {return this->_command;}
	std::vector<std::string> getParameters() const {return this->_parameters;}
};
std::ostream &operator<<(std::ostream &o, Message const &m);
#endif