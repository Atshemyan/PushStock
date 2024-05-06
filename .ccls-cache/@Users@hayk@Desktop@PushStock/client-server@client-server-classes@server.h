#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <ctime>
#include "coin.hpp"
class Server
{
public:
    Server();
    void creatingSocket();
    void openDataBase();
    void openTransactionDataBase();
    void listenAndDo();
    void sendResponse(int clientSocket, const char* response);
    ~Server();

private:
    int m_serverSocket {};
    int clientSocket {};
    struct sockaddr_in m_serverAddr;
    struct sockaddr_in m_clientAddr;
    socklen_t m_addrLen = sizeof(sockaddr_in);
    sqlite3 *m_db {};
    sqlite3 *m_transactionDb;
    std::atomic<bool> m_shouldTerminate {};
    std::thread m_updateThread;
    std::vector<Coin*> m_coinVec { new Vhuk, new Hke, new NarSmoke, new Voyl };
    void updateAllCoins();
    double getMoneyFromDB(const std::string& mail, sqlite3 *db);
    void setMoneyToDB(double newMoney, const std::string& mail, sqlite3 *db);
    void updateCoinSlot(const std::string& username, sqlite3 *db, const std::string& coinName, double num, int index);
    double getCoinCount(const std::string& username, sqlite3* db, const std::string& coinName);
};