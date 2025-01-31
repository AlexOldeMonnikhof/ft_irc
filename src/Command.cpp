#include "Server.hpp"

Command::Command(const std::string &str)
{
    parseCmd(str);
}

void  Command::parseCmd(std::string str)
{
    std::string word = "";
    bool colonFound = false;
    _cmd.clear();
    if (str.find_first_not_of(WHITE_SPACE) == std::string::npos)
    {
        return;
    }

    for (size_t i = 0; i < str.size(); i++)
    {
        if (colonFound)
        {
            word += str[i];
        }
        else if (std::isspace(str[i]))
        {
            if (!word.empty())
            {
                _cmd.push_back(word);
                word.clear();
            }
        } else if (str[i] == ':' && !colonFound)
        {
                colonFound = true;
                if (!word.empty())
                {
                    _cmd.push_back(word);
                }
                word.clear();
        } else
        {
            word += str[i];
        }
    }

    word.erase(0, word.find_first_not_of(WHITE_SPACE));
    if (!word.empty())
    {
        _cmd.push_back(word);
    }
}

std::vector<std::string>  Command::getV() const
{
    return _cmd;
}

std::string  Command::getCmd(int i) const
{
    return _cmd[i];
}

void    Command::setCmd(int i, const std::string &arg)
{
    _cmd[i] = arg;
}

size_t  Command::getSize() const
{
    return _cmd.size();
}
