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

string  Client::getNickname() const
{
    return (_nickname);
}

void    Client::setNickname(string nickname)
{
    _nickname = nickname;
}

string  Client::getUsername() const
{
    return (_username);
}

void    Client::setUsername(string username)
{
    _username = username;
}

string  Client::getHostname() const
{
    return (_hostname);
}

void    Client::setHostname(string hostname)
{
    _hostname = hostname;
}

string  Client::getServername() const
{
    return (_servername);
}

void    Client::setServername(string servername)
{
    _servername = servername;
}

string  Client::getRealname() const
{
    return (_realname);
}

void    Client::setRealname(string realname)
{
    _realname = realname;
}
