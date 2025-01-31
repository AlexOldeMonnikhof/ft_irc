#include "../inc/Chatbot.hpp"

Chatbot::Chatbot() {};

Chatbot::Chatbot(const Chatbot &rhs)
{
    (void)rhs;
}

Chatbot &Chatbot::operator=(const Chatbot &rhs)
{
    if (this != &rhs) {}

    return *this;
}

std::string Chatbot::getRespons()
{
    return "Sorry i'm still learning so i can not help you on the moment";
}
