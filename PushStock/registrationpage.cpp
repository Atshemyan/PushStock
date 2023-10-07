#include "registrationpage.h"
#include "server.h"

RegistrationPage::RegistrationPage(QWidget *parent)
    : QWidget{parent}
{
    auto size = parentWidget()->size();
    this->setGeometry(10, 10, size.width(), size.height());

    //login page header
    QLabel *topLabel = new QLabel(this);
    topLabel->setGeometry(0, 0, size.width(), 30);
    m_btnBack = new QPushButton(topLabel);
    m_btnBack->setText("← Back");
    m_btnBack->setStyleSheet("QPushButton { background: transparent; color: white; }"
                            "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnBack->setGeometry(0, 0, 65, 30);

    //registration page logo slot  (in center)
    QLabel *logoLabel = new QLabel(this);
    //300 is a png size, Vardanator sorry for magic number i will fix it later
    int logoLabelWidth = (size.width() / 2) - 160;
    int logoLabelHeight =  (size.height() / 2) - 320;
    logoLabel->setGeometry(logoLabelWidth, logoLabelHeight - 8, 300, 300);
    logoLabel->setStyleSheet("background: url('/Users/hayk/Desktop/1.png')");

    QLabel *textLabel = new QLabel(this);

    QLabel *nameLabel= new QLabel(textLabel);
    nameLabel->setText("Name:");

    QLabel *surnameLabel= new QLabel(textLabel);
    surnameLabel->setText("Surname:");

    QLabel *phoneLabel= new QLabel(textLabel);
    phoneLabel->setText("Phone:");

    QLabel *mailLabel= new QLabel(textLabel);
    mailLabel->setText("E-mail:");

    QLabel *passwordLabel = new QLabel(textLabel);
    passwordLabel->setText("password:");

    m_txtName = new QLineEdit(textLabel);
    m_txtSurname = new QLineEdit(textLabel);
    m_txtPhone = new QLineEdit(textLabel);
    m_txtUser = new QLineEdit(textLabel);
    m_txtPassword = new QLineEdit(textLabel);
    m_btnReg = new QPushButton(textLabel);
    m_btnReg->setText("Register");


    int textLabelWidth = logoLabelWidth + 75;
    int textLabelHeight = logoLabelHeight + 310;
    textLabel->setGeometry(textLabelWidth - 75, textLabelHeight, 300, 400);
    unsigned int padding = 0;

    nameLabel->setGeometry(0, 0, 300, 20);
    m_txtName->setGeometry(0, padding += 30, 300, 23);

    surnameLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtSurname->setGeometry(0, padding += 30, 300, 23);

    phoneLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtPhone->setGeometry(0, padding += 30, 300, 23);

    mailLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtUser->setGeometry(0, padding += 30, 300, 23);

    passwordLabel->setGeometry(0, padding += 30, 300, 20);
    m_txtPassword->setGeometry(0, padding += 30, 300, 23);
    m_txtPassword->setEchoMode(QLineEdit::Password);

    m_btnReg->setGeometry(0, padding += 40, 300, 35);

    QObject::connect(m_btnBack, &QPushButton::clicked, [&]()
    {
        deleteLater();
    });
    QObject::connect(m_btnReg, &QPushButton::clicked, [&]()
    {
        Server::getInstance().createSocket();
        Server::getInstance().connectToServer(); //need destructor to close Socket
        std::string command = "REGISTER;";
        command += m_txtName->text().toStdString() + ';';
        command += m_txtSurname->text().toStdString() + ';';
        command += m_txtUser->text().toStdString() + ';';
        command += m_txtPhone->text().toStdString() + ';';
        command += m_txtPassword->text().toStdString() + ';';
        Server::getInstance().sendCommandToServer(command);
        Server::getInstance().closeSocket();
        deleteLater();
    });

}

