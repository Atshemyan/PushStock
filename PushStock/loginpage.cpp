#include "loginpage.h"
#include "server.h"


LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    auto size = parentWidget()->size();
    this->setGeometry(0, 0, size.width(), size.height());

    //login page header
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

    //login information

    QLabel *textLabel = new QLabel(this);

    QLabel *userLabel= new QLabel(textLabel);
    userLabel->setText("e-mail:");
    QLabel *passwordLabel = new QLabel(textLabel);
    passwordLabel->setText("password:");

    m_txtUser = new QLineEdit(textLabel);
    m_txtPassword = new QLineEdit(textLabel);
    m_btnLogin = new QPushButton(textLabel);
    m_btnLogin->setText("Login");


    int textLabelWidth = logoLabelWidth + 75;
    int textLabelHeight = logoLabelHeight + 310;
    textLabel->setGeometry(textLabelWidth - 75, textLabelHeight, 300, 300);
    unsigned int padding = 0;
    userLabel->setGeometry(0, 0, 300, 20);
    m_txtUser->setGeometry(0, padding += 30, 300, 23);

    passwordLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtPassword->setGeometry(0, padding += 30, 300, 23);
    m_txtPassword->setEchoMode(QLineEdit::Password);


    m_btnLogin->setGeometry(0, padding + 40, 300, 35);


    QObject::connect(m_btnLogin, &QPushButton::clicked, [&]()
    {
        Server::getInstance().createSocket();
        Server::getInstance().connectToServer(); //need destructor to close Socket
        std::string command = "LOGIN;";
        command += m_txtUser->text().toStdString() + ';';
        command += m_txtPassword->text().toStdString() + ';';
        Server::getInstance().sendCommandToServer(command);
        if (Server::getInstance().responseFromServer() == "OK")
        {
            //showing stockPage
            StockPage *m_stockPage = new StockPage(this, m_txtUser->text().toStdString());
            m_stockPage->show();
        }

        Server::getInstance().closeSocket();

    });


}


