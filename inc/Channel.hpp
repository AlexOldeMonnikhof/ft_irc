#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <vector>
#include "Server.hpp"

using namespace std;

class Channel{
    private:
        string              _name;
        string              _password;
        map<string, bool>   _clients;
    public:
        Channel(){};
        Channel(int fd, string nick, string channelName);

        void    join(int fd, string nick);
        void    part(int fd, string nick);

        bool    clientInChannel(string nick);

        string  getName() const;
        void    setName(string name);
        string  getPassword() const;
        void    setPassword(string password);
        size_t  getClientsSize() const;
        vector<string>  getClients();

        void    printClients();
        bool    isOperator(const string nick);
};

#endif