#include "loginpage.h"
#include "server.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(0, 0, m_parentSize.width(), m_parentSize.height());

    this->pageHeader();
    this->pageLogo();
    this->pageLogin();
    this->loginButtonConnect();
}

void LoginPage::loginButtonConnect()
{
    QObject::connect(m_btnLogin, &QPushButton::clicked, [&]()
    {
        EmailValidator mailValidator;
        PasswordValidator passwordValidator;
        m_txtUser->setValidator(&mailValidator);
        m_txtPassword->setValidator(&passwordValidator);

        // Use non-temporary QString variables for validation
        QString mail = m_txtUser->text();
        QString password = m_txtPassword->text();

        int pos {};
        bool validState {true};

        if (m_txtUser->validator()->validate(mail, pos) != QValidator::Acceptable)
        {
            m_txtUser->setStyleSheet("border: 1px solid red;");
            validState = false;
        }
        if (m_txtPassword->validator()->validate(password, pos) != QValidator::Acceptable)
        {
            m_txtPassword->setStyleSheet("border: 1px solid red;");
            validState = false;
        }
        if (validState)
        {
            Server::getInstance().createSocket();
            Server::getInstance().connectToServer();
            LoginBuilder(m_txtUser->text().toStdString(), m_txtPassword->text().toStdString());
            if (Server::getInstance().responseFromServer() == "OK")
            {
                //showing stockPage
                m_stockPage = new StockPage(this, m_txtUser->text().toStdString());
                m_stockPage->show();
            }
            else
            {
                m_txtUser->setStyleSheet("border: 1px solid red;");
                m_txtPassword->setStyleSheet("border: 1px solid red;");
            }

            Server::getInstance().closeSocket();
        }
        else
        {
            m_btnLogin->setStyleSheet("background-color: red; color: white;");  // Set button to red when declined.
        }
    });

}

void LoginPage::pageHeader()
{
    m_topLabel = new QLabel(this);
    m_topLabel->setGeometry(10, 10, m_parentSize.width(), 30);

    m_btnBack = new QPushButton(m_topLabel);
    m_btnBack->setText("← Back");
    m_btnBack->setStyleSheet("QPushButton { background: transparent; color: white; }"
                            "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnBack->setGeometry(0, 0, 65, 30);
    QObject::connect(m_btnBack, &QPushButton::clicked, [&]()
    {
        deleteLater();
    });
}

void LoginPage::pageLogo()
{
    m_logoLabel = new QLabel(this);
    int logoLabelWidth = (m_parentSize.width() / 2) - 150;
    int logoLabelHeight =  ((m_parentSize.height() / 2) - 170) - 8;
    m_logoLabel->setGeometry(logoLabelWidth, logoLabelHeight, 300, 300);
    m_logoLabel->setStyleSheet("background: url(':images/logo.png')");
}

void LoginPage::pageLogin()
{
    QLabel *textLabel = new QLabel(this);

    QLabel *userLabel= new QLabel(textLabel);
    userLabel->setText("e-mail:");
    QLabel *passwordLabel = new QLabel(textLabel);
    passwordLabel->setText("password:");

    m_txtUser = new QLineEdit(textLabel);
    m_txtUser->setFocus();
    m_txtPassword = new QLineEdit(textLabel);
    m_btnLogin = new QPushButton(textLabel);
    m_btnLogin->setText("Login →");
    m_btnLogin->setStyleSheet("QPushButton { background: transparent; color: white; }"
                             "QPushButton:hover { color: rgb(241, 180, 41); }");


    int textLabelWidth = ((m_parentSize.width() / 2) - 150) + 75; //logoLabelWidth + 75
    int textLabelHeight = (((m_parentSize.height() / 2) - 170) - 8) + 310; //logoLabelWi
    textLabel->setGeometry(textLabelWidth - 75, textLabelHeight, 300, 300);
    unsigned int padding = 0;
    userLabel->setGeometry(0, 0, 300, 20);
    m_txtUser->setGeometry(0, padding += 30, 300, 23);

    passwordLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtPassword->setGeometry(0, padding += 30, 300, 23);
    m_txtPassword->setEchoMode(QLineEdit::Password);
    m_btnLogin->setGeometry(0, padding + 40, 300, 35);
}
