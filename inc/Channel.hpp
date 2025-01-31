#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <vector>
#include "Server.hpp"

class Channel{
    private:
        std::string                 _name;
        std::string                 _password;
        std::map<std::string, bool> _clients;
        int                         _userlimit;
        bool                        _inviteOnly;
        std::vector<std::string>    _inviteList;
        bool                        _topicOnlyOperators;
        std::string                 _topic;
    public:
        Channel(){};
        Channel(int fd, std::string nick, std::string channelName);

        void                        join(int fd, std::string nick);
        void                        part(int fd, std::string nick);

        bool                        clientInChannel(std::string nick);

        std::string                 getName() const;
        void                        setName(std::string name);
        std::string                 getPassword() const;
        void                        setPassword(std::string password);
        size_t                      getClientsSize() const;
        void                        setUserLimit(int limit);
        int                         getUserLimit() const;
        bool                        getInviteOnly() const;
        void                        setInviteOnly(bool inviteOnly);
        void                        inviteUser(std::string nick);
        bool                        isUserInvited(std::string nick) const;
        void                        setOperator(std::string nick);
        void                        removeOperator(std::string nick);
        std::string                 getTopic() const;
        void                        setTopic(std::string topic);
        bool                        topicOnlyOperators() const;
        void                        setTopicOnlyOperators(bool topicOnlyOperators);
        bool                        isFull() const;

        std::vector<std::string>    getClients();

        void                        printClients();
        bool                        isOperator(const std::string nick);
};

#endif
