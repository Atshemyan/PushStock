#ifndef REGISTERBUILDER_H
#define REGISTERBUILDER_H

#include "messagebuilder.h"
#include "server.h"

class RegisterBuilder : public MessageBuilder
{
public:
    RegisterBuilder(const std::string& name, const std::string& surname,
                    const std::string& mail,const std::string& phone, const std::string& password);
private:
    std::string m_name;
    std::string m_surname;
    std::string m_mail;
    std::string m_phone;
    std::string m_password;
    void sendCommandToServer() override;
};

#endif // REGISTERBUILDER_H
