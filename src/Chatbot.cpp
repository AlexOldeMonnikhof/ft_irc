#include <cstdlib>
#include <ctime>
#include <string>

#include "Chatbot.hpp"

Chatbot::Chatbot(const std::string &nickname) : _nickname(nickname)
{
    initCommands();
};

Chatbot::Chatbot(const Chatbot &rhs) :
    _nickname(rhs._nickname),
    _commands(rhs._commands)
{}

Chatbot &Chatbot::operator=(const Chatbot &rhs)
{
    if (this != &rhs) {
        _nickname = rhs._nickname;
        _commands = rhs._commands;
    }

    return *this;
}

Chatbot::~Chatbot() {}

const std::string &Chatbot::getNickname() const
{
    return _nickname;
}

std::string Chatbot::getRespons(const std::string &sender, const std::string &message)
{
    if (sender == _nickname)
    {
        return "";
    }

    if (message.length() > 0 && message[0] == '!')
    {
        std::string command = message.substr(1);
        std::map<std::string, std::string>::const_iterator it = _commands.find(command);
        if (it != _commands.end())
        {
            return it->second;
        }
    }

    if (message.find("hello") != std::string::npos || message.find("hi") != std::string::npos)
    {
        return "Hello " + sender + "! How can I help you today?";
    }

    return "Sorry i'm still in mine learning fase, and like my mentor i'm a slow learner, please have some patcience with me";
}

void Chatbot::initCommands()
{
    _commands["help"] = "Available commands: !help, !time, !about, !rules";
    _commands["time"] = "Current server time: " + getCurrentTime();
    _commands["about"] = "I'm a friendly IRC bot here to help!";
    _commands["rules"] = "1. Be respectful 2. No spam 3. Have fun!";
}

std::string Chatbot::getCurrentTime()
{
    time_t now = time(0);
    return std::string(ctime(&now));
}
