#ifndef CLIENT_HPP
#define CLIENT_HPP

#define FOREVER -1

#define NICKNAME 0
#define USERNAME 1
#define PASSWORD 2

#include <string>

class Client{
    private:
        int         _socket;
        int         _register;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _servername;
        std::string _realname;
    public:
        Client(int socket);
        Client(int fd, const std::string &nick, const std::string &channelName);
        Client(const Client &rhs);
        Client &operator=(const Client &rhs);
        ~Client();

    public:
        int         getRegister() const;
        void        setRegister(int value);
        std::string getNickname() const;
        void        setNickname(const std::string &nickname);
        std::string getUsername() const;
        void        setUsername(const std::string &username);
        std::string getHostname() const;
        void        setHostname(const std::string &hostname);
        std::string getServername() const;
        void        setServername(const std::string &servername);
        std::string getRealname() const;
        void        setRealname(const std::string &realname);
};

#endif
