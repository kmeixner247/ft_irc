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
}
void Message::clear()
{
    this->_prefix = "";
    this->_command = "";
    this->_parameters.clear();
}

void Message::parse(std::string input)
{
    if (input[0] == ':')
    {
        this->_prefix = input.substr(0, input.find_first_of(" "));
        input = input.substr(input.find_first_of(" ") + 1, input.npos);
    }
    else
        this->_prefix = "";
    this->_command = input.substr(0, input.find_first_of(" "));
    input = input.substr(input.find_first_of(" ") + 1, input.npos);
    size_t pos;
    while ((pos = input.find_first_of(" ")) != input.npos)
    {
        this->_parameters.push_back(input.substr(0, pos));
        input = input.substr(pos + 1, input.npos);
    }
    this->_parameters.push_back(input.substr(0, pos));
}