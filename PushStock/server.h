#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Server
{
public:
    static Server& getInstance();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    void createSocket();
    void connectToServer();
    void sendCommandToServer(const std::string& command);
    void closeSocket();
    std::string responseFromServer();
    std::string getMoney(const std::string& username);

private:
    Server();
    static Server m_instance;
    static std::mutex m_mutex;
    int m_clientSocket {};
    struct sockaddr_in m_serverAddr;


};

#endif // SERVER_H
