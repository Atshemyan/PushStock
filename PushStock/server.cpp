#include "server.h"
Server Server::m_instance;
std::mutex Server::m_mutex;

Server& Server::getInstance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_instance;
}

void Server::createSocket()
{
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
}

void Server::connectToServer()
{
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(4444);
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    //    m_serverAddr.sin_addr.s_addr = inet_addr("192.313.123");


    if (connect(m_clientSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }
}

void Server::sendCommandToServer(const std::string& command)
{
    ssize_t bytesSent = send(m_clientSocket, command.c_str(), command.size(), 0);
    if (bytesSent == -1) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }
}

std::string Server::responseFromServer()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesReceived = recv(m_clientSocket, buffer, sizeof(buffer), 0);

    return std::string(buffer);

}

int Server::getSocket() const {
    return m_clientSocket;
}

std::string Server::getMoney(const std::string& username)
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer();
    Server::getInstance().sendCommandToServer("GETMONEY;" + username + ';');
    std::string response = responseFromServer();

    size_t dotPos = response.find('.');
    if (dotPos != std::string::npos)
    {
        size_t numDecimals = response.size() - dotPos - 1;

        if (numDecimals > 1)
        {
            response = response.substr(0, dotPos + 2);
        }
    }
    Server::getInstance().closeSocket();
    return response;
}

void Server::closeSocket()
{
    close(m_clientSocket);

}

Server::Server()
{

}


