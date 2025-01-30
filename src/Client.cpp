#include "Client.hpp"

Client::Client(int socket)
{
    _socket = socket;
    _register = 0;
}

int Client::getRegister() const
{
    return (_register);
}

void    Client::setRegister(int value)
{
    if (value == PASSWORD)
        _register = _register | 0b100;
    if (value == NICKNAME)
        _register = _register | 0b10;
    if (value == USERNAME)
        _register = _register | 0b1;
}

std::string  Client::getNickname() const
{
    return (_nickname);
}

void    Client::setNickname(std::string nickname)
{
    _nickname = nickname;
}

std::string  Client::getUsername() const
{
    return (_username);
}

void    Client::setUsername(std::string username)
{
    _username = username;
}

std::string  Client::getHostname() const
{
    return (_hostname);
}

void    Client::setHostname(std::string hostname)
{
    _hostname = hostname;
}

std::string  Client::getServername() const
{
    return (_servername);
}

void    Client::setServername(std::string servername)
{
    _servername = servername;
}

std::string  Client::getRealname() const
{
    return (_realname);
}

void    Client::setRealname(std::string realname)
{
    _realname = realname;
}
