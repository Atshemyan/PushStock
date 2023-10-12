#include "depositpage.h"

DepositPage::DepositPage(QWidget *parent, const std::string& useranme)
    : QWidget {parent}
    , m_currentUserName {useranme}
{
    m_size = parentWidget()->size();
    this->setGeometry(0, 0, m_size.width(), m_size.height());
    this->pageHeader();
    this->pageLogo();
    this->depositOpportunity();
    m_txtMoney->setFocus();

}

void DepositPage::pageHeader()
{
    QLabel *topLabel = new QLabel(this);
    topLabel->setGeometry(10, 10, m_size.width(), 30);
    m_btnBack = new QPushButton(topLabel);
    m_btnBack->setText("← Back");
    m_btnBack->setStyleSheet("QPushButton { background: transparent; color: white; }"
                            "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnBack->setGeometry(0, 0, 65, 30);
    QObject::connect(m_btnBack, &QPushButton::clicked, [&]()
    {
        deleteLater();
    });
}

void DepositPage::pageLogo()
{
    QLabel *logoLabel = new QLabel(this);
    int logoLabelWidth = (m_size.width() / 2) - 150;
    int logoLabelHeight =  ((m_size.height() / 2) - 170) - 8;
    logoLabel->setGeometry(logoLabelWidth, logoLabelHeight, 300, 300);
    logoLabel->setStyleSheet("background: url(':images/logo.png')");
}

void DepositPage::doDeposit(const std::string& money)
{
    Server::getInstance().createSocket();
    Server::getInstance().connectToServer();
    std::string command = "DEPOSIT;" + m_currentUserName + ';' + money + ';';

    Server::getInstance().sendCommandToServer(command);
    std::string response = Server::getInstance().responseFromServer();
    if (response == "OK")
    {
        deleteLater();
    }
    else if (response == "!OK")
    {
        m_txtMoney->setStyleSheet("border: 1px solid red;");
    }

    Server::getInstance().closeSocket();
}

void DepositPage::depositOpportunity()
{
    QLabel *depositLabel = new QLabel(this);
    depositLabel->setGeometry((m_size.width() / 2) - 150, (((m_size.height() / 2) - 170) - 8) + 310, 300, 200);
    QLabel *moneyLabel = new QLabel(depositLabel);
    moneyLabel->setGeometry(0, 0, 60, 40);
    moneyLabel->setText("$USD");
    m_txtMoney = new QLineEdit(depositLabel);
    m_txtMoney->setGeometry(0, 30, 300, 23);
    m_btnDeposit = new QPushButton(depositLabel);
    m_btnDeposit->setGeometry(0, 60, 300, 35);
    m_btnDeposit->setText("Deposit");
    m_btnDeposit->setStyleSheet("QPushButton {"
                                "background-color: darkorange;"
                                "color: white;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: orange;"
                                "}");
    QObject::connect(m_btnDeposit, &QPushButton::clicked, [&]()
    {
        DepositValidator depositValidator;
        int pos {};
        m_txtMoney->setValidator(&depositValidator);
        QString money = m_txtMoney->text();

        if (m_txtMoney->validator()->validate(money, pos) != QValidator::Acceptable)
        {
            m_txtMoney->setStyleSheet("border: 1px solid red;");
        }
        else
        {
            doDeposit(m_txtMoney->text().toStdString());
        }
    });
}
