#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <poll.h>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

#define WAIT_FOREVER -1

#define BUFFER_LENGTH 2048

class Server{
    private:
        int                         _port;
        int                         _socket;
        std::string                 _password;
        std::string                 _host;
        std::vector<pollfd>         _fds;
        std::map<int, Client>       _clients;
        std::vector<Channel>        _channels;
    public:
        Server(const std::string &port, const std::string &password);
        Server(const Server &rhs);
        Server &operator=(const Server &rhs);
        ~Server();

    public:
        void                        parseServer(const std::string &port, const std::string &password);
        void                        initServer();
        void                        addClient();
        void                        disconnectClient(int fd);

        void                        cmdPASS(int fd, Command& cmd);
        bool                        isValidName(std::string nick);
        bool                        nickInUse(int fd, const std::string &nick);
        void                        cmdNICK(int fd, Command& cmd);
        void                        cmdUSER(int fd, Command& cmd);

        void                        cmdJoin(int fd, Command& cmd);
        void                        cmdPart(int fd, Command& cmd);
        void                        cmdMode(int fd, Command& cmd);
        void                        cmdKick(int fd, Command& cmd);
        void                        cmdQuit(int fd, Command& cmd);

        void                        cmdInvite(int fd, Command& cmd);
        void                        modeInviteOnly(int fd, Channel& channel, bool adding);

        void                        cmdTopic(int fd, Command& cmd);
        void                        modeTopic(int fd, Channel& channel, bool adding);

        void                        modeUserLimit(int fd, Channel& channel, bool adding, Command& cmd);

        void                        modeKey(int fd, Channel& channel, bool adding, Command& cmd);

        void                        modeOperator(int fd, Channel& channel, bool adding, Command& cmd);

        void                        privmsgChannel(int fd, Command& cmd, const std::string &channel);
        void                        privmsgClient(int fd, Command& cmd, const std::string &nick);
        void                        cmdPrivmsg(int fd, Command& cmd);
        void                        msgChannel(int fd, const std::string &msg, const std::string &channel, bool sendToSender);

        void                        registerClient(int fd, Command& cmd);
        void                        cmdsClient(int fd, Command& cmd);
        void                        handleMsgClient(int fd);
        void                        handleHexChatRegister(int fd, const std::string &buffer);

        bool                        channelExist(const std::string &channel);
        int                         getClientFd(const std::string &nick);
        void                        msgAllClients(const std::string &msg);

        void                        mainLoop();
};
void                                sendMsg(int fd, const std::string &msg);
std::vector<std::string>            splitVector(const std::string &s, char delimiter);
size_t                              getChannelIndex(std::vector<Channel>& channels, const std::string &name);
#endif
