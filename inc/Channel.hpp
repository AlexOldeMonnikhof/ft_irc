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
        Channel(int fd, const std::string &nick, const std::string &channelName);
        Channel(const Channel &rhs);
        Channel &operator=(const Channel &rhs);
        ~Channel();

    public:
        void                        join(int fd, const std::string &nick);
        void                        part(int fd, const std::string &nick);

        bool                        clientInChannel(const std::string &nick);

        std::string                 getName() const;
        void                        setName(const std::string &name);
        std::string                 getPassword() const;
        void                        setPassword(const std::string &password);
        size_t                      getClientsSize() const;
        void                        setUserLimit(int limit);
        int                         getUserLimit() const;
        bool                        getInviteOnly() const;
        void                        setInviteOnly(bool inviteOnly);
        void                        inviteUser(const std::string &nick);
        bool                        isUserInvited(const std::string &nick) const;
        void                        setOperator(const std::string &nick);
        void                        removeOperator(const std::string &nick);
        std::string                 getTopic() const;
        void                        setTopic(const std::string &topic);
        bool                        topicOnlyOperators() const;
        void                        setTopicOnlyOperators(bool topicOnlyOperators);
        bool                        isFull() const;

        std::vector<std::string>    getClients();

        void                        printClients();
        bool                        isOperator(const std::string &nick);
};

#endif
