#include <iostream>

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircsercv port password" << '\n';
        return 1;
    }

    try
    {
        Server server(argv[1], argv[2]);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
