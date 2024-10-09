#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "Server.hpp"

using namespace std;

class Channel{
    private:
        string          _name;
        string          _password;
        map<string, bool>  _clients;
    public:
        Channel(){};
        Channel(int fd, string username, string channelName);

        void    join(int fd, string username);
        void    part(int fd, string username);

        string  getName() const;
        void    setName(string name);
        string  getPassword() const;
        void    setPassword(string password);
        size_t  getClientsSize() const;

        void    printClients();
};

#endif