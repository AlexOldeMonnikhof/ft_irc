#include "Server.hpp"

size_t getChannelIndex(std::vector<Channel>& channels, std::string name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].getName() == name)
        {
            return i;
        }
    }

    return std::string::npos;
}

bool    Server::channelExist(std::string channel)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == channel)
        {
            return true;
        }
    }

    return false;
}

bool    Server::isValidName(std::string nick)
{
    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];
        if (i == 0 && isdigit(nick[i]))
        {
            return false;
        }

        if (!isalnum(c) && c != '[' && c != ']' && c != '{' && c != '}' && c != '\\' && c != '|' && c != '-')
        {
            return false;
        }
    }

    return true;
}

bool    Server::nickInUse(int fd, std::string nick)
{
    for (std::vector<pollfd>::iterator iter = _fds.begin() + 1; iter < _fds.end(); iter++)
    {
        if (iter->fd == fd)
        {
            continue;
        }

        if (_clients[iter->fd].getNickname() == nick)
        {
            return true;
        }

    }

    return false;
}

int     Server::getClientFd(std::string nick)
{
    for (std::map<int, Client>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        if (iter->second.getNickname() == nick)
        {
            return iter->first;
        }
    }

    return 0;
}
