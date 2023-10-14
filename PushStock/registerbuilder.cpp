#include "registerbuilder.h"
#include "decrypt.h"
#include <QDebug>

RegisterBuilder::RegisterBuilder(const std::string& name, const std::string& surname,
        const std::string& mail,const std::string& phone, const std::string& password)
    :   m_name {name}
    ,   m_surname {surname}
    ,   m_mail {mail}
    ,   m_phone {phone}
    ,   m_password {password}
{
    this->sendCommandToServer();
}

void RegisterBuilder::sendCommandToServer()
{
    std::string command = "REGISTER;";
    command += m_name + ';';
    command += m_surname + ';';
    command += m_mail + ';';
    command += m_phone + ';';
    command += std::to_string(decrypt(m_password)) + ';';

    Server::getInstance().sendCommandToServer(command);
}
