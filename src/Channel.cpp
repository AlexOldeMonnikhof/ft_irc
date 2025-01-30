#include "Channel.hpp"

Channel::Channel(int fd, string nick, string channelName)
{
    _clients[nick] = true;
    _userlimit = 50;
    _inviteOnly = false;
    _topicOnlyOperators = false;
    _topic = "";
    cout << nick << " (" << fd <<") created channel " << channelName << endl;
    setName(channelName);
}

void    Channel::join(int fd, string nick)
{
    if (_clients.find(nick) != _clients.end())
        sendMsg(fd, "Already in channel\n\r");
    else
    {
        _clients[nick] = false;
        cout << nick << " joined channel " << _name << endl;
    }
}

void    Channel::part(int fd, string nick, string _host, map<int, Client>& clientsFds)
{
    bool wasOp = isOperator(nick);
    _clients.erase(nick);
    if (wasOp && !_clients.empty())
    {
        _clients.begin()->second = true;
        sendMsg(clientsFds.begin()->first, RPL_YOUREOPER(_host, _clients.begin()->first));
    }
    cout << nick << " (" << fd <<") left channel " << _name << endl;
}

bool    Channel::clientInChannel(string nick)
{
    if (_clients.find(nick) != _clients.end())
        return true;
    return false;
}

string  Channel::getName() const
{
    return _name;
}

void    Channel::setName(string name)
{
    _name = name;
}

string  Channel::getPassword() const
{
    return _password;
}

void    Channel::setPassword(string password)
{
    _password = password;
}

size_t  Channel::getClientsSize() const
{
    return _clients.size();
}

vector<string> Channel::getClients()
{
    vector<string>  clients;

    for (map<string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
        clients.push_back(iter->first);
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

string  Channel::getTopic() const
{
    return _topic;
}

void    Channel::setTopic(string topic)
{
    _topic = topic;
}

void    Channel::printClients()
{
    for (map<string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
        cout << iter->first << endl;
}

void    Channel::inviteUser(string nick)
{
    _inviteList.push_back(nick);
}

void    Channel::setOperator(string nick)
{
    _clients[nick] = true;
}

void    Channel::removeOperator(string nick)
{
    _clients[nick] = false;
}

bool    Channel::isFull() const
{
    if (_userlimit && _clients.size() >= (size_t)_userlimit)
        return true;
    return false;
}

bool    Channel::isUserInvited(string nick) const
{
    for (size_t i = 0; i < _inviteList.size(); i++)
    {
        if (_inviteList[i] == nick)
            return true;
    }
    return false;
}

bool    Channel::isOperator(const string nick)
{
    std::map<std::string, bool>::const_iterator it = _clients.find(nick);
    if (it != _clients.end())
        return it->second;
    return false;
}
