#ifndef CLIENT_HPP
#define CLIENT_HPP

#define FOREVER -1

#define NICKNAME 0
#define USERNAME 1
#define PASSWORD 2

#include <string>

using namespace std;

class Client{
    private:
        int             _socket;
        int             _register;
        string          _nickname;
        string          _username;
        string          _hostname;
        string          _servername;
        string          _realname;
    public:
        Client(){};
        Client(int socket);
        int         getRegister() const;
        void        setRegister(int value);
        string      getNickname() const;
        void        setNickname(string nickname);
        string      getUsername() const;
        void        setUsername(string username);
        string      getHostname() const;
        void        setHostname(string hostname);
        string      getServername() const;
        void        setServername(string servername);
        string      getRealname() const;
        void        setRealname(string realname);
};

#endif