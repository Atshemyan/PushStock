#include "stockpage.h"

StockPage::StockPage(QWidget *parent, const std::string& username)
    : QWidget {parent}
    , m_currentUserUsername {username}
{
    m_size = parentWidget()->size();
    this->setGeometry(0, 0, m_size.width(), m_size.height());

    this->initTopLabel();
    this->moneyShow();
    this->initStockLabel();
    this->coinSelection();
    this->initCoinPrice();
    this->createEmptyChart();
    this->priceGraphic();
    this->initDepositButton();
    this->initExitButton();
    this->initBotLabel();
    this->initBotButtons();
    this->initTransactionList();
    this->coinShow();

    m_priceChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);

}

void StockPage::initBotLabel()
{
    m_botLabel = new QLabel(this);
    m_botLabel->setGeometry(0, m_stockLabel->height() + 50, m_size.width(), m_size.height() - (m_stockLabel->height() + m_topLabel->height()));
}

void StockPage::initBotButtons()
{
    m_btnBuy = new QPushButton(m_botLabel);
    m_btnBuy->setGeometry(m_botLabel->width() - (140 + 140 + 60), 5, 140, 40);

    m_btnBuy->setText("Buy");
    m_btnBuy->setStyleSheet(
        "QPushButton {"
        "   background-color: #008000; /* Green */"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #00A000; /* Slightly Lighter Green on Hover */"
        "}"

        );


    m_btnSell = new QPushButton(m_botLabel);
    m_btnSell->setGeometry(m_botLabel->width() - 170, 5, 140, 40);
    m_btnSell->setText("Sell");
    m_btnSell->setStyleSheet(
        "QPushButton {"
        "   background-color: #8B0000; /* Dark Red */"
        "   color: white;"
        "}"
        "QPushButton:hover {"
        "   background-color: #A52A2A; /* Slightly Lighter Red on Hover */"
        "}"
        );


    QObject::connect(m_btnBuy, &QPushButton::clicked, this, [&]()
    {
        bool ok {false};
        QString userInput;
        Server::getInstance().createSocket();
        Server::getInstance().connectToServer();
        Server::getInstance().sendCommandToServer("GETMONEY;" + m_currentUserUsername + ';');


        double money = std::stod(Server::getInstance().responseFromServer());
        Server::getInstance().closeSocket();

        QDoubleValidator validator(0.5, money, 6, this);  // Example: validate to allow values between 0 and 10000 with 6 decimal places
        validator.setLocale(QLocale::C); // Set the C (POSIX) locale


        do
        {
            userInput = QInputDialog::getText(this, "Buy", "USD$:", QLineEdit::Normal, "", &ok);
            if (ok)
            {
                int pos = 0;
                if (validator.validate(userInput, pos) != QValidator::Acceptable)
                {
                    ok = false;
                    QMessageBox::warning(this, "Invalid Input", "Please enter a valid amount.");
                }
            }
        } while (!ok && !userInput.isEmpty());

        if (ok)
        {
            Server::getInstance().createSocket();
            Server::getInstance().connectToServer();
            Server::getInstance().sendCommandToServer("BUY;" + m_coinsSelection->currentText().toStdString() + ';' + userInput.toStdString() + ';' + m_currentUserUsername + ';');
            Server::getInstance().closeSocket();

        }
    });

    QObject::connect(m_btnSell, &QPushButton::clicked, [&]()
    {
        bool ok {false};
        QString userInput;
        Server::getInstance().createSocket();
        Server::getInstance().connectToServer();
        Server::getInstance().sendCommandToServer("GETCOINCOUNT;" + m_currentUserUsername + ';' + m_coinsSelection->currentText().toStdString() + ';');
        std::string response = Server::getInstance().responseFromServer();

        double coinCount = std::stod(response);
        Server::getInstance().closeSocket();

        QDoubleValidator validator(0.5, coinCount, 6, this);  // Example: validate to allow values between 0 and 10000 with 6 decimal places
        validator.setLocale(QLocale::C); // Set the C (POSIX) locale


        do
        {
            userInput = QInputDialog::getText(this, "Sell", m_coinsSelection->currentText(), QLineEdit::Normal, "", &ok);
            if (ok)
            {
                int pos = 0;
                if (validator.validate(userInput, pos) != QValidator::Acceptable)
                {
                    ok = false;
                    QMessageBox::warning(this, "Invalid Input", "Please enter a valid amount.");
                }
            }
        } while (!ok && !userInput.isEmpty());

        if (ok)
        {
            Server::getInstance().createSocket();
            Server::getInstance().connectToServer();
            Server::getInstance().sendCommandToServer("SELL;" + m_coinsSelection->currentText().toStdString() + ';' + userInput.toStdString() + ';' + m_currentUserUsername + ';');
            Server::getInstance().closeSocket();

        }
    });


}

void StockPage::settingCoinPrice()
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer();
    Server::getInstance().sendCommandToServer("GET" + m_coinsSelection->currentText().toStdString() + "PRICE;");
    std::string response = Server::getInstance().responseFromServer();
    this->removeDots(response);
    double tmpPrice = std::stod(response);
    if (tmpPrice > m_prevPriceOfCoin)
    {

//        QPen pen(QColor(0, 255, 0));
        m_coinPrice->setStyleSheet("color: green");
//        m_priceSeries->setPen(pen);
        m_priceSeries->setColor(QColor(0, 255, 0));

    }
    else
    {
//        QPen pen(QColor(255, 0, 0));
        m_coinPrice->setStyleSheet("color: red");
//        m_priceSeries->setPen(pen);
        m_priceSeries->setColor(QColor(255, 0, 0));



    }

    m_coinPrice->setText(QString::fromStdString('$' + response));
    m_prevPriceOfCoin = tmpPrice;


    this->updateChart(response);
    QObject::connect(m_coinsSelection, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index)
    {
        this->resetChart();

    });
    Server::getInstance().closeSocket();
}

void StockPage::priceGraphic()
{
    this->settingCoinPrice();
    QTimer *coinMoneyTimer = new QTimer(this);
    connect(coinMoneyTimer, &QTimer::timeout, [=]()
    {
        this->settingCoinPrice();

    });

    coinMoneyTimer->start(1000);
}

void StockPage::initExitButton()
{
    m_btnExit = new QPushButton(m_topLabel);
    m_btnExit->setText("Exit →");
    m_btnExit->setGeometry(m_size.width() - 170, 0, 140, 40);
    m_btnExit->setStyleSheet("QPushButton { background: transparent; color: white; }"
                             "QPushButton:hover { color: rgb(241, 180, 41); }");
    QObject::connect(m_btnExit, &QPushButton::clicked, [&]()
    {
        deleteLater();
    });
}

void StockPage::initDepositButton()
{
    m_btnDeposit = new QPushButton(m_topLabel);
    m_btnDeposit->setText("Deposit");
    m_btnDeposit->setGeometry(m_size.width() - (140 + 140 + 60), 0, 140, 40);
    m_btnDeposit->setStyleSheet("QPushButton {"
                                "background-color: darkorange;"
                                "color: white;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: orange;"
                                "}");
    QObject::connect(m_btnDeposit, &QPushButton::clicked, [&]()
    {
        m_depositPage = new DepositPage(this, m_currentUserUsername);
        m_depositPage->show();
    });
}

void StockPage::createEmptyChart()
{
    m_priceSeries = new QLineSeries();
    m_priceChart = new QChart();

    m_priceChart->addSeries(m_priceSeries);

    QPen pen(QColor(241, 180, 41));
    pen.setWidth(4); // Set the line width
    m_priceSeries->setPen(pen);
    QLegend *legend = m_priceChart->legend();
    legend->hide();


    m_priceChart->setBackgroundBrush(QBrush(Qt::transparent));
    m_priceChart->createDefaultAxes();
    m_priceChartView = new QChartView(m_priceChart);
    m_priceChartView->setRenderHint(QPainter::Antialiasing);
    m_priceChartView->setGeometry((m_size.width() / 2) - m_size.width() / 2, 100, m_size.width(), 400);

    m_priceChartView->setParent(m_stockLabel);
    QDateTimeAxis *timeAxis = new QDateTimeAxis;
    timeAxis->setFormat("hh:mm:ss");
    m_priceChart->addAxis(timeAxis, Qt::AlignBottom);
    m_priceSeries->attachAxis(timeAxis);
}

void StockPage::initCoinPrice()
{
    m_coinPrice = new QLabel(m_stockLabel);
    m_coinPrice->setGeometry(140, 10, 120, 40);
    m_coinPrice->setFont(QFont("Arial", 20));


}

void StockPage::initStockLabel()
{
    m_stockLabel = new QLabel(this);
    m_stockLabel->setGeometry(0, m_topLabel->height() + 45, m_size.width(), m_size.height() - m_size.height() / 3);
}

void StockPage::coinSelection()
{
    m_coinsSelection = new QComboBox(m_stockLabel);
    m_coinsSelection->setGeometry(20, 12, 100, 40);
    m_coinsSelection->setFont(QFont("Arial", 18));

    m_coinsSelection->addItem("VHUK");
    m_coinsSelection->addItem("HKE");
    m_coinsSelection->addItem("NARSMOKE");
    m_coinsSelection->addItem("VOYL");

}

void StockPage::moneyShow()
{
    m_moneyToShow = new QLabel(m_topLabel);
    m_moneyToShow->setGeometry(20, 0, 130, 50);
    m_moneyToShow->setStyleSheet("font-size: 20px");
    m_moneyToShow->setText(('$' + Server::getInstance().getMoney(m_currentUserUsername)).c_str());

    QTimer *timerMoney = new QTimer(this);

    connect(timerMoney, &QTimer::timeout, [=]() {
        m_moneyToShow->setText(('$' + Server::getInstance().getMoney(m_currentUserUsername)).c_str());
    });

    timerMoney->start(3000);
}

void StockPage::getCoinsCount()
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer();
    Server::getInstance().sendCommandToServer("GETCOUNT;" + m_currentUserUsername + ';');
    std::string response = Server::getInstance().responseFromServer();
    m_coinsCount->setText(response.c_str());
    m_coinsCount->setStyleSheet("font-size: 20px");
    Server::getInstance().closeSocket();
}

void StockPage::coinShow()
{
    m_coinsCount = new QLabel(m_topLabel);
    m_coinsCount->setGeometry(m_moneyToShow->width() + 40, 0, 700, 50);
    QTimer *timerCoin = new QTimer(this);
    getCoinsCount();

    connect(timerCoin, &QTimer::timeout, [=]()
    {
        getCoinsCount();
    });
    timerCoin->start(2000);
}

void StockPage::initTopLabel()
{
    m_topLabel = new QLabel(this);
    m_topLabel->setGeometry(10, 40, m_size.width(), 40);

}

void StockPage::updateChart(std::string& response)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    double price = std::stod(response);

    m_priceSeries->append(currentTime.toMSecsSinceEpoch(), price);

    while (m_priceSeries->count() > 200)
    {
        m_priceSeries->remove(0);
    }

    m_priceChart->axes(Qt::Horizontal).first()->setRange(currentTime.addSecs(-100).toMSecsSinceEpoch(), currentTime.toMSecsSinceEpoch());
    m_priceChart->axes(Qt::Vertical).first()->setRange(0, 2500);

    m_priceChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    m_priceChart->update();
}

void StockPage::removeDots(std::string& response)
{
    size_t dotPos = response.find('.');
    if (dotPos != std::string::npos)
    {
        size_t numDecimals = response.size() - dotPos - 1;

        if (numDecimals > 1)
        {
            response = response.substr(0, dotPos + 2);
        }
    }
}

void StockPage::resetChart()
{
    // Clear the series data
    m_priceSeries->clear();

    // Reset the horizontal axis range
    QDateTime currentTime = QDateTime::currentDateTime();
    m_priceChart->axes(Qt::Horizontal).first()->setRange(currentTime.addSecs(-100).toMSecsSinceEpoch(), currentTime.toMSecsSinceEpoch());

    m_priceChart->update();
}

void StockPage::updateTransactionList()
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer();
    Server::getInstance().sendCommandToServer("GETTRANSACTIONS;");
    std::string response = Server::getInstance().responseFromServer();

    std::vector<std::string> newItems;

    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;

    while ((pos = response.find(delimiter)) != std::string::npos)
    {
        token = response.substr(0, pos);
        newItems.push_back(token);
        response.erase(0, pos + delimiter.length());
    }

    for (const std::string& item : newItems)
    {
        if (!m_existingItemsInList.contains(QString::fromStdString(item))) {
            QListWidgetItem *listItem = new QListWidgetItem(QString::fromStdString(item));
            if (std::strstr(item.c_str(), "BUY"))
            {
                listItem->setForeground(QColor(60, 139, 34));
            }
            else if (std::strstr(item.c_str(), "SELL"))
            {
                listItem->setForeground(QColor(150, 0, 0));
            }
            m_transactionListWidget->insertItem(0, listItem);
            m_existingItemsInList.insert(QString::fromStdString(item));
        }
    }

    Server::getInstance().closeSocket();
}

void StockPage::initTransactionList()
{
    m_transactionListWidget = new QListWidget(m_botLabel);
    m_transactionListWidget->setGeometry(80, 10, 500, m_botLabel->height() - 30);
    m_transactionListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    // Set a stylesheet for the QListWidget's vertical scroll bar
    m_transactionListWidget->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: none;"
        "    width: 8px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #FFA500;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none;"
        "}"
    );

    this->updateTransactionList();
    QTimer *timerTransaction = new QTimer(this);

    connect(timerTransaction, &QTimer::timeout, [=]()
    {
        this->updateTransactionList();
    });
    timerTransaction->start(1000);
}
