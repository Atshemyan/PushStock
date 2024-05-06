#include "server.h"
#include "getCryptoPrice.hpp"
#include <iostream>

Server::Server()
{
    creatingSocket();
    openDataBase();
    m_updateThread = std::thread(&Server::updateAllCoins, this);
    openTransactionDataBase();
    listenAndDo();
}

void updateLocalCoinPrices() {

}
void Server::updateAllCoins()
{
    while (!m_shouldTerminate)
    {   

        // m_btcPrice = "1";
        // m_ethPrice = "2";
        // m_bnbPrice = "3";

        m_btcPrice = get_crypto_price("btc");
        m_ethPrice = get_crypto_price("eth");
        m_bnbPrice = get_crypto_price("bnb");
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

}
double Server::getMoneyFromDB(const std::string& mail, sqlite3 *db)
{
    double money = 0.0;

    const char* selectMoneySQL = "SELECT Money FROM client_information WHERE Mail = ?;";
    sqlite3_stmt* moneyStmt;

    int rc = sqlite3_prepare_v2(db, selectMoneySQL, -1, &moneyStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return money;
    }

    sqlite3_bind_text(moneyStmt, 1, mail.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(moneyStmt);
    if (rc == SQLITE_ROW)
    {
        money = sqlite3_column_double(moneyStmt, 0);
    }

    sqlite3_finalize(moneyStmt);

    return money;
}

double Server::getCoinCount(const std::string& username, sqlite3* db, const std::string& coinName)
{
    std::string selectCoinCountSQL = "SELECT " + coinName + " FROM client_information WHERE Mail = ?;";
    sqlite3_stmt* selectCoinCountStmt;

    int rc = sqlite3_prepare_v2(db, selectCoinCountSQL.c_str(), -1, &selectCoinCountStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    sqlite3_bind_text(selectCoinCountStmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(selectCoinCountStmt);
    if (rc == SQLITE_ROW)
    {
        double coinCount = sqlite3_column_double(selectCoinCountStmt, 0);
        sqlite3_finalize(selectCoinCountStmt);
        return coinCount;
    }
    else
    {
        std::cerr << "Error getting coin count!" << std::endl;
        sqlite3_finalize(selectCoinCountStmt);
        return -1; 
    }
}


void Server::updateCoinSlot(const std::string& username, sqlite3 *db, const std::string& coinName, double num)
{    
    std::string updateCoinSlotSQL = "UPDATE client_information SET " + coinName + " = ? WHERE Mail = ?;";
    sqlite3_stmt* updateCoinSlotStmt;

    int rc = sqlite3_prepare_v2(db, updateCoinSlotSQL.c_str(), -1, &updateCoinSlotStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_double(updateCoinSlotStmt, 1, num);
    sqlite3_bind_text(updateCoinSlotStmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(updateCoinSlotStmt);
    if (rc != SQLITE_DONE)
    {
        perror("Error executing SQL statement");
    }
    sqlite3_finalize(updateCoinSlotStmt);
}

void Server::setMoneyToDB(double newMoney, const std::string& mail, sqlite3* db)
{
    const char* updateMoneySQL = "UPDATE client_information SET Money = ? WHERE Mail = ?;";
    sqlite3_stmt* updateMoneyStmt;

    int rc = sqlite3_prepare_v2(db, updateMoneySQL, -1, &updateMoneyStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_double(updateMoneyStmt, 1, newMoney);
    sqlite3_bind_text(updateMoneyStmt, 2, mail.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(updateMoneyStmt);
    if (rc != SQLITE_DONE)
    {
        perror("Error executing SQL statement");
    }

    sqlite3_finalize(updateMoneyStmt);
}


Server::~Server()
{
    
    m_shouldTerminate = true;

    if (m_updateThread.joinable())
    {
        m_updateThread.join();
    }
    
}

void Server::creatingSocket()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(4444);
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(m_serverSocket, 10) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }
}

void Server::openDataBase()
{
    int rc = sqlite3_open("../PushStock.db", &m_db);
    if (rc != SQLITE_OK) {
        perror("Error opening database");
        exit(EXIT_FAILURE);
    }
}

void Server::openTransactionDataBase()
{
    int rc = sqlite3_open("../transactions.db", &m_transactionDb);
    if (rc != SQLITE_OK) {
        perror("Error opening database");
        exit(EXIT_FAILURE);
    }
}




void Server::sendResponse(int clientSocket, const char* response)
{
    ssize_t bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent == -1)
    {
        perror("Error sending response");
    }
}


void Server::listenAndDo()
{
    while(true)
    {

        clientSocket = accept(m_serverSocket, (struct sockaddr*)&m_clientAddr, &m_addrLen);
        
        if (clientSocket == -1) {
            perror("Error accepting client connection");
            continue;
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived == -1) {
            perror("Error receiving data");
            close(clientSocket);
            continue;
        }

        std::string dataReceived(buffer);
        std::string delimiter = ";";
        size_t pos = 0;
        std::string token;
        std::vector<std::string> parameters;

        while ((pos = dataReceived.find(delimiter)) != std::string::npos)
        {
            token = dataReceived.substr(0, pos);
            parameters.push_back(token);
            dataReceived.erase(0, pos + delimiter.length());
        }

        std::string operation = parameters[0];        
        
        if (operation == "REGISTER")
        {
            std::string name = parameters[1];
            std::string surname = parameters[2];
            std::string mail = parameters[3];
            std::string phoneNumber = parameters[4];
            std::string password = parameters[5];
            const char* insertSQL = "INSERT INTO client_information (Name, Surname, Phone, Mail, Password, Money, BTC, ETH, BNB) VALUES (?, ?, ?, ?, ?, 0, 0, 0 ,0);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_db, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue;
            }

            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, phoneNumber.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, mail.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, password.c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE)
            {
                std::cerr << "Execution failed: " << sqlite3_errmsg(m_db) << std::endl;
                sqlite3_finalize(stmt);
                close(clientSocket);
                continue;
            }
            std::cout << "Registered! \n";
            sendResponse(clientSocket, "OK");
            
            sqlite3_finalize(stmt);


        }
        else if (operation == "LOGIN")
        {
            std::string username = parameters[1];
            std::string password = parameters[2];
            const char* selectSQL = "SELECT * FROM client_information WHERE Mail = ? AND Password = ?;";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_db, selectSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK)
            {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                return; 
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW)
            {
                std::cout << "Login successful!\n";
                sendResponse(clientSocket, "OK");
            }
            else
            {
                std::cout << "Login failed!\n";
                sendResponse(clientSocket, "!OK");
            }
            sqlite3_finalize(stmt);
        }

        else if (operation == "GETMONEY")
        {
            std::string mail = parameters[1];
            sendResponse(clientSocket, std::to_string(getMoneyFromDB(mail, m_db)).c_str());

        }

        else if (operation == "DEPOSIT")
        {
            std::string mail = parameters[1];
            double depositAmount = std::stod(parameters[2]);

            double money = getMoneyFromDB(mail, m_db);
            if (depositAmount > 0)
            {
                setMoneyToDB(depositAmount + money, mail, m_db);
                sendResponse(clientSocket, "OK");
            }
            else
            {
                sendResponse(clientSocket, "!OK");
            }
            
           
        }

        else if (operation == "GETBTCPRICE")
        {
            sendResponse(clientSocket, m_btcPrice.c_str());
        }

        else if (operation == "GETETHPRICE")
        {
            sendResponse(clientSocket, m_ethPrice.c_str());
        }

        else if (operation == "GETBNBPRICE")
        {
            sendResponse(clientSocket, m_bnbPrice.c_str());
        }

        else if (operation == "BUY")
        {
            std::string coinName = parameters[1];
            double purchaseAmount = std::stod(parameters[2]);
            std::string username = parameters[3];

            double money = getMoneyFromDB(username, m_db);
            money -= purchaseAmount;
            setMoneyToDB(money, username, m_db);
            double price = -1;
            int index = 0;
            if (coinName == "BTC") {
                price = std::stod(m_btcPrice);
            }
            else if (coinName == "ETH") {
                price = std::stod(m_ethPrice);
                index = 1;
            }
            else {
                price = std::stod(m_bnbPrice);
                index = 2;
            }
            updateCoinSlot(username, m_db, coinName, purchaseAmount / price);
            
            const char* insertSQL = "INSERT INTO transactions_information (username, coin, usdcoincount, price, buysell, date) VALUES (?, ?, ?, ?, ?, ?);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_transactionDb, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue;
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, coinName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, std::to_string(purchaseAmount).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, std::to_string(price).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, "BUY", -1, SQLITE_STATIC);
             std::time_t currentTime = std::time(nullptr);

            std::tm *currentDateTime = std::localtime(&currentTime);

            std::string date = std::to_string(currentDateTime->tm_year + 1900) + '-'
                            + std::to_string(currentDateTime->tm_mon + 1) + '-'
                            + std::to_string(currentDateTime->tm_mday) + '|';

            std::string time = std::to_string(currentDateTime->tm_hour) + ':'
                            + std::to_string(currentDateTime->tm_min) + ':'
                            + std::to_string(currentDateTime->tm_sec);

            sqlite3_bind_text(stmt, 6, (date + time).c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                perror("Error executing SQL statement");
            }
            std::cout << "BUYED! \n";
            
            sqlite3_finalize(stmt);
        }

        else if (operation == "SELL")
        {
            std::string coinName = parameters[1];
            double coinSellCount = std::stod(parameters[2]);
            std::string username = parameters[3];

            
            double price = -1;
            int index = 0;
            if (coinName == "BTC") {
                price = std::stod(m_btcPrice);
            }
            else if (coinName == "ETH") {
                price = std::stod(m_ethPrice);
                index = 1;
            }
            else {
                price = std::stod(m_bnbPrice);
                index = 2;
            }
            double money = getMoneyFromDB(username, m_db);
            money += coinSellCount * price;
            setMoneyToDB(money, username, m_db);
            double coinCount = getCoinCount(username, m_db, coinName);
            updateCoinSlot(username, m_db, coinName, coinCount - coinSellCount);
            
            const char* insertSQL = "INSERT INTO transactions_information (username, coin, usdcoincount, price, buysell, date) VALUES (?, ?, ?, ?, ?, ?);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_transactionDb, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue;
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, coinName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, std::to_string(coinCount).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, std::to_string(price).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, "SELL", -1, SQLITE_STATIC);
             std::time_t currentTime = std::time(nullptr);

            std::tm *currentDateTime = std::localtime(&currentTime);

            std::string date = std::to_string(currentDateTime->tm_year + 1900) + '-'
                            + std::to_string(currentDateTime->tm_mon + 1) + '-'
                            + std::to_string(currentDateTime->tm_mday) + '|';

            std::string time = std::to_string(currentDateTime->tm_hour) + ':'
                            + std::to_string(currentDateTime->tm_min) + ':'
                            + std::to_string(currentDateTime->tm_sec);

            sqlite3_bind_text(stmt, 6, (date + time).c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                perror("Error executing SQL statement");
            }            
            sqlite3_finalize(stmt);
            std::cout << "SELLED! \n";

        }

        else if (operation == "GETCOUNT")
        {
            std::string username = parameters[1];
            std::string message;

            message +=  "  BTC: " + std::to_string(getCoinCount(username, m_db, "BTC"));
            message +=  "  ETH: " + std::to_string(getCoinCount(username, m_db, "ETH"));
            message +=  "  BNB: " + std::to_string(getCoinCount(username, m_db, "BNB"));
            sendResponse(clientSocket, message.c_str());
        }

        else if (operation == "GETCOINCOUNT")
        {
            std::string username = parameters[1];
            std::string coinName = parameters[2];
            std::string message = std::to_string(getCoinCount(username, m_db, coinName));
            sendResponse(clientSocket, message.c_str());
        }

        else if (operation == "GETTRANSACTIONS")
        {
            std::string transactions;

            const char* selectAllTransactionsSQL = "SELECT * FROM transactions_information;";
            sqlite3_stmt* transactionsStmt;

            int rc = sqlite3_prepare_v2(m_transactionDb, selectAllTransactionsSQL, -1, &transactionsStmt, nullptr);
            if (rc != SQLITE_OK)
            {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_transactionDb) << std::endl;
            }

            unsigned int lineCounter {};

            while ((rc = sqlite3_step(transactionsStmt)) == SQLITE_ROW)
            {
                for (int col = 0; col < sqlite3_column_count(transactionsStmt); ++col)
                {
                    const char* value = reinterpret_cast<const char*>(sqlite3_column_text(transactionsStmt, col));
                    transactions += value;
                    transactions += ' ';
                }
                transactions += ";";
                ++lineCounter;
            }
            
            if (lineCounter >= 15)
            {
                const char* deleteOldestTransactionSQL = "DELETE FROM transactions_information WHERE date = (SELECT MIN(date) FROM transactions_information);";

                int deleteRc = sqlite3_exec(m_transactionDb, deleteOldestTransactionSQL, nullptr, nullptr, nullptr);
                if (deleteRc != SQLITE_OK)
                {
                    std::cerr << "Error deleting the oldest transaction: " << sqlite3_errmsg(m_transactionDb) << std::endl;
                }
            }
            
            if (rc != SQLITE_DONE)
            {
                std::cerr << "Error executing SQL statement" << std::endl;
            }
            sqlite3_finalize(transactionsStmt);
            sendResponse(clientSocket, transactions.c_str());

        }

        close(clientSocket);

    }
}
