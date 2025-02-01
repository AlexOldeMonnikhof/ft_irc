#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#define WHITE_SPACE "\f\n\r\t\v "

class Command{
    private:
        std::vector<std::string>    _cmd;
    public:
        Command(const std::string &str);
        Command(const Command &rhs);
        Command &operator=(const Command &rhs);
        ~Command();

    public:
        void                        parseCmd(const std::string &str);
        std::vector<std::string>    getV() const;
        void                        setCmd(int i, const std::string &arg);
        std::string                 getCmd(int i) const;
        size_t                      getSize() const;
};

#endif  // COMMAND_HPP
