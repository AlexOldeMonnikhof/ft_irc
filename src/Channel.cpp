#include "Channel.hpp"

Channel::Channel(int fd, string username, string channelName)
{
    _clients[username] = true;
    cout << fd << " created channel " << channelName << endl;
    setName(channelName);
}

void    Channel::join(int fd, string username)
{
    if (_clients.find(username) != _clients.end())
        sendMsg(fd, "Already in channel\n\r");
    else
    {
        _clients[username] = false;
        cout << username << " joined channel " << _name << endl;
    }
}

void    Channel::part(int fd, string username)
{
    _clients.erase(username);
    cout << "Client " << username << " left channel " << _name << endl;
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

void    Channel::printClients()
{
    for (map<string, bool>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
        cout << iter->first << endl;
}