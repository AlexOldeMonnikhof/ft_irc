#include "Channel.hpp"

Channel::Channel(int fd, string nick, string channelName)
{
    _clients[nick] = true;
    cout << fd << " created channel " << channelName << endl;
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

void    Channel::part(int fd, string nick)
{
    _clients.erase(nick);
    cout << "Client " << nick << " left channel " << _name << endl;
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
    cout << "clients size: " << _clients.size() << endl;
    return _clients.size();
}

vector<string> Channel::getClients()
{
    vector<string>  clients;

    for (map<string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
        clients.push_back(iter->first);
    return clients;
}

void    Channel::printClients()
{
    for (map<string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
        cout << iter->first << endl;
}