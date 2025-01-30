#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./ircsercv port password" << std::endl;
        return 1;
    }
    try
    {
        Server server(argv[1], argv[2]);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}