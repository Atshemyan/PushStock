#include "server.h"

Server::Server()
{
    creatingSocket();
    openDataBase();
    m_updateThread = std::thread(&Server::updateAllCoins, this);
    openTransactionDataBase();
    listenAndDo();
}
void Server::updateAllCoins()
{
    while (!m_shouldTerminate)
    {
        for (auto &&i : m_coinVec)
        {
            i->editPrice();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
        // Handle the error as needed
        return money; // Return 0.0 in case of an error
    }

    sqlite3_bind_text(moneyStmt, 1, mail.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(moneyStmt);
    if (rc == SQLITE_ROW)
    {
        money = sqlite3_column_double(moneyStmt, 0); // Get the money balance from the query
    }

    sqlite3_finalize(moneyStmt);

    return money;
}

double Server::getCoinCount(const std::string& username, sqlite3* db, const std::string& coinName)
{
    // Создаем SQL-запрос для получения количества монет в указанном слоте
    std::string selectCoinCountSQL = "SELECT " + coinName + " FROM client_information WHERE Mail = ?;";
    sqlite3_stmt* selectCoinCountStmt;

    int rc = sqlite3_prepare_v2(db, selectCoinCountSQL.c_str(), -1, &selectCoinCountStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        // Обработка ошибки по мере необходимости
        return -1; // Возврат -1 как признак ошибки
    }

    sqlite3_bind_text(selectCoinCountStmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(selectCoinCountStmt);
    if (rc == SQLITE_ROW)
    {
        // Получаем количество монет из результата запроса
        double coinCount = sqlite3_column_double(selectCoinCountStmt, 0);
        sqlite3_finalize(selectCoinCountStmt);
        return coinCount;
    }
    else
    {
        // Пользователь не найден в базе данных или другая ошибка
        std::cerr << "Error getting coin count!" << std::endl;
        sqlite3_finalize(selectCoinCountStmt);
        return -1; // Возврат -1 как признак ошибки
    }
}


void Server::updateCoinSlot(const std::string& username, sqlite3 *db, const std::string& coinName, double num, int index)
{    
    // Создаем SQL-запрос для обновления значения в соответствующем столбце
    std::string updateCoinSlotSQL = "UPDATE client_information SET " + m_coinVec[index]->getName() + " = ? WHERE Mail = ?;";
    sqlite3_stmt* updateCoinSlotStmt;

    int rc = sqlite3_prepare_v2(db, updateCoinSlotSQL.c_str(), -1, &updateCoinSlotStmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        // Обработка ошибки по мере необходимости
        return;
    }

    sqlite3_bind_double(updateCoinSlotStmt, 1, num);
    sqlite3_bind_text(updateCoinSlotStmt, 2, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(updateCoinSlotStmt);
    if (rc != SQLITE_DONE)
    {
        perror("Error executing SQL statement");
        // Обработка ошибки по мере необходимости
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
        // Handle the error as needed
        return;
    }

    sqlite3_bind_double(updateMoneyStmt, 1, newMoney);
    sqlite3_bind_text(updateMoneyStmt, 2, mail.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(updateMoneyStmt);
    if (rc != SQLITE_DONE)
    {
        perror("Error executing SQL statement");
        // Handle the error as needed
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
    m_serverAddr.sin_port = htons(7778); // Choose a port
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(m_serverSocket, 6) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }
}

void Server::openDataBase()
{
    int rc = sqlite3_open("/Users/hayk/Desktop/client-server/PushStock.db", &m_db);
    if (rc != SQLITE_OK) {
        perror("Error opening database");
        exit(EXIT_FAILURE);
    }
}

void Server::openTransactionDataBase()
{
    int rc = sqlite3_open("/Users/hayk/Desktop/client-server/transactions.db", &m_transactionDb);
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
        // std::cout << "Listening! \n";
        // Accept incoming client connections
        clientSocket = accept(m_serverSocket, (struct sockaddr*)&m_clientAddr, &m_addrLen);
        std::cout << "Client connected\n";

        if (clientSocket == -1) {
            perror("Error accepting client connection");
            continue; // Continue listening for other connections
        }

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Receive data from the connected client
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived == -1) {
            perror("Error receiving data");
            close(clientSocket);
            continue; // Continue listening for other connections
        }

        // Parse the received data
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

        // Extract individual parameters
        std::string operation = parameters[0];        
        
        //commands
        if (operation == "REGISTER")
        {
            std::string name = parameters[1];
            std::string surname = parameters[2];
            std::string mail = parameters[3];
            std::string phoneNumber = parameters[4];
            std::string password = parameters[5];
            std::cout << password << std::endl;
            // Insert data into the SQLite database table
            const char* insertSQL = "INSERT INTO client_information (Name, Surname, Phone, Mail, Password, Money, VHUK, HKE, NARSMOKE, VOYL) VALUES (?, ?, ?, ?, ?, 0, 0, 0 ,0, 0);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_db, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue; // Continue listening for other connections
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
                return; // Exit or handle the error as appropriate
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW)
            {
                // Запись найдена в базе данных
                std::cout << "Login successful!\n";
                sendResponse(clientSocket, "OK");
            }
            else
            {
                // Запись не найдена в базе данных
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
            double depositAmount = std::stod(parameters[2]); // Преобразование строки в double

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

        else if (operation == "GETVHUKPRICE")
        {
            sendResponse(clientSocket, std::to_string(m_coinVec[0]->getPrice()).c_str());
        }

        else if (operation == "GETHKEPRICE")
        {
            sendResponse(clientSocket, std::to_string(m_coinVec[1]->getPrice()).c_str());
        }

        else if (operation == "GETNARSMOKEPRICE")
        {
            sendResponse(clientSocket, std::to_string(m_coinVec[2]->getPrice()).c_str());
        }

        else if (operation == "GETVOYLPRICE")
        {
            sendResponse(clientSocket, std::to_string(m_coinVec[3]->getPrice()).c_str());
        }

        else if (operation == "BUY")
        {
            
            std::string coinName = parameters[1];
            double purchaseAmount = std::stod(parameters[2]);
            std::string username = parameters[3];

            double money = getMoneyFromDB(username, m_db);
            money -= purchaseAmount;
            setMoneyToDB(money, username, m_db);
            int index {-1};
            for (int i = 0; i < m_coinVec.size(); ++i)
            {
                if (m_coinVec[i]->getName() == coinName)
                {
                    index = i;
                }
                
            }
            updateCoinSlot(username, m_db, coinName, purchaseAmount / m_coinVec[index]->getPrice(), index);
            //connect to transactions db and set information there
            
            const char* insertSQL = "INSERT INTO transactions_information (username, coin, usdcoincount, price, buysell, date) VALUES (?, ?, ?, ?, ?, ?);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_transactionDb, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue; // Continue listening for other connections
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, coinName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, std::to_string(purchaseAmount).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, std::to_string(m_coinVec[index]->getPrice()).c_str(), -1, SQLITE_STATIC);
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

            
            int index {-1};
            for (int i = 0; i < m_coinVec.size(); ++i)
            {
                if (m_coinVec[i]->getName() == coinName)
                {
                    index = i;
                }
                
            }
            double money = getMoneyFromDB(username, m_db);
            money += coinSellCount * m_coinVec[index]->getPrice();
            setMoneyToDB(money, username, m_db);
            double coinCount = getCoinCount(username, m_db, coinName);
            updateCoinSlot(username, m_db, coinName, coinCount - coinSellCount , index);
             //connect to transactions db and set information there
            
            const char* insertSQL = "INSERT INTO transactions_information (username, coin, usdcoincount, price, buysell, date) VALUES (?, ?, ?, ?, ?, ?);";
            sqlite3_stmt* stmt;

            int rc = sqlite3_prepare_v2(m_transactionDb, insertSQL, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << std::endl;
                close(clientSocket);
                continue; // Continue listening for other connections
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, coinName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, std::to_string(coinCount).c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, std::to_string(m_coinVec[index]->getPrice()).c_str(), -1, SQLITE_STATIC);
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

        else if (operation == "GETCOUNT") // all coins count in account
        {
            std::string username = parameters[1];
            std::string message;
            for (int i = 0; i < m_coinVec.size(); ++i)
            {
                message += m_coinVec[i]->getName() + ": ";
                message += std::to_string(getCoinCount(username, m_db, m_coinVec[i]->getName())) + "  ";
            }
            sendResponse(clientSocket, message.c_str());
        }

        else if (operation == "GETCOINCOUNT") //only 1(selected) coin count
        {
            // Server::getInstance().sendCommandToServer("GETCOINCOUNT;" + m_currentUserUsername + ';' + m_coinsSelection->currentText() + ';');
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
                // Handle the error as needed
            }

            unsigned int lineCounter {};

            while ((rc = sqlite3_step(transactionsStmt)) == SQLITE_ROW)
            {
                // Concatenate all fields for each transaction
                for (int col = 0; col < sqlite3_column_count(transactionsStmt); ++col)
                {
                    const char* value = reinterpret_cast<const char*>(sqlite3_column_text(transactionsStmt, col));
                    transactions += value;
                    transactions += ' ';
                }
                // Add a newline character to separate transactions
                transactions += ";";
                ++lineCounter;
            }
            
            if (lineCounter >= 20)
            {
                // Remove the oldest transaction
                const char* deleteOldestTransactionSQL = "DELETE FROM transactions_information WHERE date = (SELECT MIN(date) FROM transactions_information);";

                int deleteRc = sqlite3_exec(m_transactionDb, deleteOldestTransactionSQL, nullptr, nullptr, nullptr);
                if (deleteRc != SQLITE_OK)
                {
                    std::cerr << "Error deleting the oldest transaction: " << sqlite3_errmsg(m_transactionDb) << std::endl;
                    // Handle the error as needed
                }
            }
            
            if (rc != SQLITE_DONE)
            {
                std::cerr << "Error executing SQL statement" << std::endl;
                // Handle the error as needed
            }
            sqlite3_finalize(transactionsStmt);
            sendResponse(clientSocket, transactions.c_str());

        }

        close(clientSocket); // Close the client socket when done processing

    }
}
