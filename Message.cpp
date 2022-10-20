#include "Message.hpp"
#include <iostream>
Message::Message() : _prefix(""), _command(""), _parameters(std::vector<std::string>())
{
}

Message::Message(const Message &rhs)
{
	*this = rhs;
}

Message::Message(const std::string &rhs)
{
	this->parse(rhs);
}

Message::~Message()
{
	this->_parameters.clear();
}

Message &Message::operator=(const Message &rhs)
{
	this->_prefix = rhs._prefix;
	this->_command = rhs._command;
	this->_parameters = rhs._parameters;
	return (*this);
}
void Message::clear()
{
	this->_prefix = "";
	this->_command = "";
	this->_parameters.clear();
}

std::string Message::getRaw() const
{
	std::string temp = "";
	if (this->_prefix.length() > 0)
		temp = ":" + this->_prefix;
	if (this->_command.length() > 0)
	{
		if (temp.length() > 0)
			temp += " ";
		temp += this->_command;
	}
	if (this->_parameters.size() > 0)
	{
		if (temp.length() > 0)
			temp += " ";
		for (size_t i = 0; i < this->_parameters.size(); i++)
			temp += this->_parameters[i] + " ";
		temp.erase(temp.end() - 1);
		temp += "\r\n";
	}
	return (temp);
}

void Message::parse(std::string input)
{
	if (input[0] == ':')
	{
		this->_prefix = input.substr(1, input.find(' '));
		input.erase(0, input.find(' ') + 1);
	}
	else
		this->_prefix = "";
	this->_command = input.substr(0, input.find(' '));
	if (input.find(' ') == input.npos)
		return;
	input.erase(0, input.find(' ') + 1);
	size_t pos;
	while ((pos = input.find(' ')) != input.npos && input[0] != ':')
	{
		this->_parameters.push_back(input.substr(0, pos));
		input = input.erase(0, pos + 1);
	}
	if (input[0] == ':')
		input = input.erase(0, 1);
	this->_parameters.push_back(input);
}
std::ostream &operator<<(std::ostream &o, Message const &m)
{
	size_t paramsize = m.getParameters().size();
	std::cout << "PREFIX: " << m.getPrefix() << "; COMMAND: " << m.getCommand() << "; PARAMS: [";
	if (paramsize)
		std::cout << m.getParameters()[0];
	for (size_t i = 1; i < paramsize; i++)
		std::cout << ", " << m.getParameters()[i];
	std::cout << "]";
	return (o);
}