#include "depositpage.h"

DepositPage::DepositPage(QWidget *parent, const std::string& useranme)
    : QWidget {parent}
    , m_currentUserName {useranme}
{
    auto size = parentWidget()->size();
    this->setGeometry(0, 0, size.width(), size.height());
    //header
    QLabel *topLabel = new QLabel(this);
    topLabel->setGeometry(10, 10, size.width(), 30);
    m_btnBack = new QPushButton(topLabel);
    m_btnBack->setText("← Back");
    m_btnBack->setStyleSheet("QPushButton { background: transparent; color: white; }"
                            "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnBack->setGeometry(0, 0, 65, 30);
    QObject::connect(m_btnBack, &QPushButton::clicked, [&]()
    {
        deleteLater();
    });

    //login page logo slot  (in center)
    QLabel *logoLabel = new QLabel(this);
    //300 is a png size, Vardanator sorry for magic number i will fix it later
    int logoLabelWidth = (size.width() / 2) - 150;
    int logoLabelHeight =  ((size.height() / 2) - 170) - 8;
    logoLabel->setGeometry(logoLabelWidth, logoLabelHeight, 300, 300);
    logoLabel->setStyleSheet("background: url('/Users/hayk/Desktop/1.png')");

//    Deposit
    QLabel *depositLabel = new QLabel(this);
    depositLabel->setGeometry(logoLabelWidth, logoLabelHeight + 310, 300, 200);
    QLabel *moneyLabel = new QLabel(depositLabel);
    moneyLabel->setGeometry(0, 0, 60, 40);
    moneyLabel->setText("$USD");
    m_txtMoney = new QLineEdit(depositLabel);
    m_txtMoney->setGeometry(0, 30, 300, 23);
    m_btnDeposit = new QPushButton(depositLabel);
    m_btnDeposit->setGeometry(0, 60, 300, 35);
    m_btnDeposit->setText("Deposit");
    QObject::connect(m_btnDeposit, &QPushButton::clicked, [&]()
    {
        doDeposit(m_txtMoney->text().toStdString());
    });

}

void DepositPage::doDeposit(const std::string& money)
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer(); //need destructor to close Socket
    std::string command = "DEPOSIT;" + m_currentUserName + ';' + money + ';';

    Server::getInstance().sendCommandToServer(command);
    if (Server::getInstance().responseFromServer() == "OK")
    {

    }

    Server::getInstance().closeSocket();
    deleteLater();
}
