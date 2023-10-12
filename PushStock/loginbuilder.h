#ifndef LOGINBUILDER_H
#define LOGINBUILDER_H
#include "messagebuilder.h"
#include "server.h"


class LoginBuilder : public MessageBuilder
{
public:
    LoginBuilder(const std::string& username, const std::string& password);
private:
    std::string m_username;
    std::string m_password;
    void sendCommandToServer() override;
};

#endif // LOGINBUILDER_H
