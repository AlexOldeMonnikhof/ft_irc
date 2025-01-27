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
        int                 _userlimit;
        bool                _inviteOnly;
        vector<string>      _inviteList;
    public:
        Channel(){};
        Channel(int fd, string nick, string channelName);

        void    join(int fd, string nick);
        void    part(int fd, string nick, string _host, map<int, Client>& clientsFds);

        bool    clientInChannel(string nick);

        string  getName() const;
        void    setName(string name);
        string  getPassword() const;
        void    setPassword(string password);
        size_t  getClientsSize() const;
        void    setUsersLimit(int limit);
        int     getUsersLimit() const;
        bool    getInviteOnly() const;
        void    setInviteOnly(bool inviteOnly);
        void    inviteUser(string nick);
        bool    isUserInvited(string nick) const;
        void    setOperator(string nick);

        vector<string>  getClients();

        void    printClients();
        bool    isOperator(const string nick);
};

#endif