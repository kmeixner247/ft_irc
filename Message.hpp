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

    //just temporary
    std::string getPrefix() {return this->_prefix;}
    std::string getCommand() {return this->_command;}
    std::vector<std::string> getParameters() {return this->_parameters;}
};
#endif