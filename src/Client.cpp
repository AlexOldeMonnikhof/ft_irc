#include "Client.hpp"

Client::Client(int socket)
{
    _socket = socket;
    _register = 0;
}

int Client::getRegister() const
{
    return (_register);
}

void    Client::setRegister(int value)
{
    if (value == NICKNAME)
        _register = _register | 0b1;
    if (value == USERNAME)
        _register = _register | 0b10;
    if (value == PASSWORD)
        _register = _register | 0b100;
}
