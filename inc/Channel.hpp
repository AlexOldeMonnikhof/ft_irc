#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "Server.hpp"

using namespace std;

class Channel{
    private:
        string          _name;
        map<int, bool>  _clients;
    public:
        Channel(){};
        Channel(int fd, string name);

        void    join(int fd);
        string  getName() const;
        void    setName(string name);
};

#endif