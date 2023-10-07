#include "stockpage.h"
#include "server.h"

StockPage::StockPage(QWidget *parent, const std::string& username)
    : QWidget {parent}
    , m_currentUserUsername {username}
{
    auto size = parentWidget()->size();
    this->setGeometry(0, 0, size.width(), size.height());

    //StockPage header

    //left side buttons
    QLabel *topLabel = new QLabel(this);
    topLabel->setGeometry(10, 10, size.width(), 40);
    m_moneyToShow = new QLabel(topLabel);
    m_moneyToShow->setGeometry(20, 0, 70, 40);
    m_moneyToShow->setText(('$' + Server::getInstance().getMoney(username)).c_str());

    QTimer *timerMoney = new QTimer(this);

    connect(timerMoney, &QTimer::timeout, [=]() {
        m_moneyToShow->setText(('$' + Server::getInstance().getMoney(username)).c_str());
    });

    timerMoney->start(2000);

    m_coinsSelection = new QComboBox(topLabel);
    m_coinsSelection->setGeometry(100, 0, 120, 40);
    m_coinsSelection->addItem("VHUK");
    m_coinsSelection->addItem("HKE");
    m_coinsSelection->addItem("NARSMOKE");
    m_coinsSelection->addItem("VOYL");

    QLabel *stockLabel = new QLabel(this);
    stockLabel->setGeometry(0, topLabel->height() + 60, size.width(), size.height() - (topLabel->height() + 60));

    QLabel *coinPrice = new QLabel(stockLabel);
    coinPrice->setGeometry(30, 0, 300, 150);

    QTimer *coinMoney = new QTimer(this);

    connect(coinMoney, &QTimer::timeout, [=]() {
        Server::getInstance().createSocket();
        Server::getInstance().connectToServer();
        Server::getInstance().sendCommandToServer("GET" + m_coinsSelection->currentText().toStdString() + "PRICE;");

        std::string response = Server::getInstance().responseFromServer();

        size_t dotPos = response.find('.');
        if (dotPos != std::string::npos)
        {
            size_t numDecimals = response.size() - dotPos - 1;

            if (numDecimals > 1)
            {
                response = response.substr(0, dotPos + 2);
            }
        }
        coinPrice->setText(QString::fromStdString(response));

        QObject::connect(m_coinsSelection, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index)
        {

            std::string response = Server::getInstance().responseFromServer();

            size_t dotPos = response.find('.');
            if (dotPos != std::string::npos)
            {
                size_t numDecimals = response.size() - dotPos - 1;

                if (numDecimals > 1)
                {
                    response = response.substr(0, dotPos + 2);
                }
            }
            coinPrice->setText(QString::fromStdString(response));

        });
        Server::getInstance().closeSocket();

    });

    coinMoney->start(500);


    //right side buttons
    m_btnDeposit = new QPushButton(topLabel);
    m_btnDeposit->setText("Deposit");
    m_btnDeposit->setGeometry(size.width() - 170, 0, 140, 40);

    QObject::connect(m_btnDeposit, &QPushButton::clicked, [&]()
    {
        m_depositPage = new DepositPage(this, m_currentUserUsername);
        m_depositPage->show();
    });
}
