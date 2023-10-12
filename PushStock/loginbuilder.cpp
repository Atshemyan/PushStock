#include "loginbuilder.h"
#include "decrypt.h"
#include <QDebug>

LoginBuilder::LoginBuilder(const std::string& username, const std::string& password)
    :   m_username {username}
    ,   m_password {password}
{
    this->sendCommandToServer();
}

void LoginBuilder::sendCommandToServer()
{
    std::string command = "LOGIN;";
    command += m_username + ';';
    command += decrypt(m_password) + ';';
    Server::getInstance().sendCommandToServer(command);
}
