#ifndef CHATBOT_HPP
#define CHATBOT_HPP

#include <string>
class Chatbot {
    public:
        Chatbot();
        Chatbot(const Chatbot &rhs);
        Chatbot &operator=(const Chatbot &rhs);
        ~Chatbot();
    public:
        std::string getRespons();
};

#endif // CHATBOT_HPP
