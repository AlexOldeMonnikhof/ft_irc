#include "Server.hpp"

size_t getChannelIndex(vector<Channel>& channels, string name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].getName() == name)
            return i;
    }
    return string::npos;
}

bool    Server::channelExist(string channel)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == channel)
            return true;
    }
    return false;
}

bool    Server::isValidName(string nick)
{
    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];
        if (i == 0 && isdigit(nick[i]))
            return false;
        if (!isalnum(c) && c != '[' && c != ']' && c != '{' && c != '}' && c != '\\' && c != '|' && c != '-')
            return false;
    }
    return true;
}

bool    Server::nickInUse(int fd, string nick)
{
    for (vector<pollfd>::iterator iter = _fds.begin() + 1; iter < _fds.end(); iter++)
    {
        if (iter->fd == fd)
            continue;
        if (_clients[iter->fd].getNickname() == nick)
            return true;
    }
    return false;
}

int     Server::getClientFd(string nick)
{
    for (map<int, Client>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        if (iter->second.getNickname() == nick)
            return iter->first;
    }
    return 0;
}
