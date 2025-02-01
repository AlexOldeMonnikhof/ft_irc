#ifndef CHATBOT_HPP
#define CHATBOT_HPP

#include <string>
#include <map>

class Chatbot {
    private:
        std::string _nickname;
        std::map<std::string, std::string> _commands;

    private:
        void initCommands();
        std::string getCurrentTime();

    public:
        Chatbot(const std::string &nickname);

        Chatbot(const Chatbot &rhs);
        Chatbot &operator=(const Chatbot &rhs);

        ~Chatbot();

    public:
        const std::string &getNickname() const;
        std::string getRespons(const std::string &sender, const std::string &message);
};

#endif // CHATBOT_HPP
