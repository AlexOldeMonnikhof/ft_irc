#include "Channel.hpp"

Channel::Channel(int fd, string name)
{
    _clients[fd] = true;
    setName(name);
}

void    Channel::join(int fd)
{
    if (_clients.find(fd) != _clients.end())
        sendMsg(fd, "Already in channel\n\r");
    else
        _clients[fd] = false;
}

string  Channel::getName() const
{
    return _name;
}

void    Channel::setName(string name)
{
    _name = name;
}