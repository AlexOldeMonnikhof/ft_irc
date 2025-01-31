#include "Channel.hpp"

Channel::Channel(int fd, std::string nick, std::string channelName) : _userlimit(50), _inviteOnly(false), _topicOnlyOperators(false), _topic("")
{
    _clients[nick] = true;
    std::cout << nick << " (" << fd <<") created channel " << channelName << '\n';
    setName(channelName);
}

void    Channel::join(int fd, std::string nick)
{
    if (_clients.find(nick) != _clients.end())
    {
        sendMsg(fd, "Already in channel\n\r");
    } else
    {
        _clients[nick] = false;
        std::cout << nick << " joined channel " << _name << '\n';
    }
}

void    Channel::part(int fd, std::string nick)
{
    _clients.erase(nick);
    std::cout << nick << " (" << fd <<") left channel " << _name << '\n';
}

bool    Channel::clientInChannel(std::string nick)
{
    if (_clients.find(nick) != _clients.end())
    {
        return true;
    }

    return false;
}

std::string  Channel::getName() const
{
    return _name;
}

void    Channel::setName(std::string name)
{
    _name = name;
}

std::string  Channel::getPassword() const
{
    return _password;
}

void    Channel::setPassword(std::string password)
{
    _password = password;
}

size_t  Channel::getClientsSize() const
{
    return _clients.size();
}

std::vector<std::string> Channel::getClients()
{
    std::vector<std::string>  clients;

    for (std::map<std::string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        clients.push_back(iter->first);
    }

    return clients;
}

void    Channel::setUserLimit(int limit)
{
    _userlimit = limit;
}

int     Channel::getUserLimit() const
{
    return _userlimit;
}

bool    Channel::getInviteOnly() const
{
    return _inviteOnly;
}

void    Channel::setInviteOnly(bool inviteOnly)
{
    _inviteOnly = inviteOnly;
}

bool    Channel::topicOnlyOperators() const
{
    return _topicOnlyOperators;
}

void    Channel::setTopicOnlyOperators(bool topicOnlyOperators)
{
    _topicOnlyOperators = topicOnlyOperators;
}

std::string  Channel::getTopic() const
{
    return _topic;
}

void    Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void    Channel::printClients()
{
    for (std::map<std::string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        std::cout << iter->first << std::endl;
    }
}

void    Channel::inviteUser(std::string nick)
{
    _inviteList.push_back(nick);
}

void    Channel::setOperator(std::string nick)
{
    _clients[nick] = true;
}

void    Channel::removeOperator(std::string nick)
{
    _clients[nick] = false;
}

bool    Channel::isFull() const
{
    if (_userlimit && _clients.size() >= (size_t)_userlimit)
    {
        return true;
    }

    return false;
}

bool    Channel::isUserInvited(std::string nick) const
{
    for (size_t i = 0; i < _inviteList.size(); i++)
    {
        if (_inviteList[i] == nick)
        {
            return true;
        }
    }

    return false;
}

bool    Channel::isOperator(const std::string nick)
{
    std::map<std::string, bool>::const_iterator it = _clients.find(nick);
    if (it != _clients.end())
    {
        return it->second;
    }

    return false;
}
