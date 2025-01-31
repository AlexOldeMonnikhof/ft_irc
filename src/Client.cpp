#include "Client.hpp"

Client::Client() {}

Client::Client(int socket) : _socket(0), _register(0)
{
    _socket = socket;
}

Client::Client(const Client &rhs) :
    _socket(rhs._socket),
    _register(rhs._register),
    _nickname(rhs._nickname),
    _username(rhs._username),
    _hostname(rhs._hostname),
    _servername(rhs._servername),
    _realname(rhs._realname)
{}

Client &Client::operator=(const Client &rhs)
{
    if (this != &rhs) {
        _socket = rhs._socket;
        _register = rhs._register;
        _nickname = rhs._nickname;
        _username = rhs._username;
        _hostname = rhs._hostname;
        _servername = rhs._servername;
        _realname = rhs._realname;
    }

    return *this;
}

Client::~Client() {}

int Client::getRegister() const
{
    return (_register);
}

void    Client::setRegister(int value)
{
    if (value == PASSWORD)
    {
        _register = _register | 0b100;
    }

    if (value == NICKNAME)
    {
        _register = _register | 0b10;
    }

    if (value == USERNAME)
    {
        _register = _register | 0b1;
    }
}

std::string  Client::getNickname() const
{
    return (_nickname);
}

void    Client::setNickname(const std::string &nickname)
{
    _nickname = nickname;
}

std::string  Client::getUsername() const
{
    return (_username);
}

void    Client::setUsername(const std::string &username)
{
    _username = username;
}

std::string  Client::getHostname() const
{
    return (_hostname);
}

void    Client::setHostname(const std::string &hostname)
{
    _hostname = hostname;
}

std::string  Client::getServername() const
{
    return (_servername);
}

void    Client::setServername(const std::string &servername)
{
    _servername = servername;
}

std::string  Client::getRealname() const
{
    return (_realname);
}

void    Client::setRealname(const std::string &realname)
{
    _realname = realname;
}
