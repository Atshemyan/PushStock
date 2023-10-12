#include "registrationpage.h"
#include "server.h"

RegistrationPage::RegistrationPage(QWidget *parent)
    : QWidget{parent}
{
    m_size = parentWidget()->size();
    this->setGeometry(10, 10, m_size.width(), m_size.height());

    this->pageHeader();

    this->pageLogo();

    this->pageRegister();

    this->registerButtonConnect();

}

void RegistrationPage::pageHeader()
{
    QLabel *topLabel = new QLabel(this);
    topLabel->setGeometry(0, 0, m_size.width(), 30);
    m_btnBack = new QPushButton(topLabel);
    m_btnBack->setText("← Back");
    m_btnBack->setStyleSheet("QPushButton { background: transparent; color: white; }"
                            "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnBack->setGeometry(0, 0, 65, 30);

}

void RegistrationPage::pageLogo()
{
    QLabel *logoLabel = new QLabel(this);
    int logoLabelWidth = (m_size.width() / 2) - 160;
    int logoLabelHeight =  (m_size.height() / 2) - 320;
    logoLabel->setGeometry(logoLabelWidth, logoLabelHeight - 8, 300, 300);
    logoLabel->setStyleSheet("background: url(':images/logo.png')");
}

void RegistrationPage::pageRegister()
{
    //labels
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

    //line edits and register button
    m_txtName = new QLineEdit(textLabel);
    m_txtName->setFocus();
    m_txtSurname = new QLineEdit(textLabel);
    m_txtPhone = new QLineEdit(textLabel);
    m_txtUser = new QLineEdit(textLabel);
    m_txtPassword = new QLineEdit(textLabel);
    m_btnReg = new QPushButton(textLabel);
    m_btnReg->setText("Register →");
    m_btnReg->setStyleSheet("QPushButton { background: transparent; color: white; }"
                              "QPushButton:hover { color: rgb(241, 180, 41); }");

    //setting geometry
    int textLabelWidth = ((m_size.width() / 2) - 160) + 75; //logo label + 75
    int textLabelHeight = ((m_size.height() / 2) - 320) + 310; //logo label + 320
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
}

void RegistrationPage::registerButtonConnect()
{
    QObject::connect(m_btnReg, &QPushButton::clicked, [&]()
    {
        NameSurnameValidator nameValidator;
        EmailValidator mailValidator;
        PasswordValidator passwordValidator;
        PhoneValidator phoneValidator;

        m_txtName->setValidator(&nameValidator);
        m_txtSurname->setValidator(&nameValidator);
        m_txtUser->setValidator(&mailValidator);
        m_txtPhone->setValidator(&phoneValidator);
        m_txtPassword->setValidator(&passwordValidator);
        int pos {};
        bool validState {true};

        // Use non-temporary QString variables for validation
        QString name = m_txtName->text();
        QString surname = m_txtSurname->text();
        QString user = m_txtUser->text();
        QString phone = m_txtPhone->text();
        QString password = m_txtPassword->text();

        if (m_txtName->validator()->validate(name, pos) != QValidator::Acceptable)
        {
            m_txtName->setStyleSheet("border: 1px solid red;");
            validState = false;
        }
        if (m_txtSurname->validator()->validate(surname, pos) != QValidator::Acceptable)
        {
            m_txtSurname->setStyleSheet("border: 1px solid red;");  // Fixed this. It should be m_txtSurname not m_txtName.
            validState = false;
        }
        if (m_txtUser->validator()->validate(user, pos) != QValidator::Acceptable)
        {
            m_txtUser->setStyleSheet("border: 1px solid red;");
            validState = false;
        }
        if (m_txtPhone->validator()->validate(phone, pos) != QValidator::Acceptable)
        {
            m_txtPhone->setStyleSheet("border: 1px solid red;");
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
            RegisterBuilder(name.toStdString(), surname.toStdString(),
                         user.toStdString(), phone.toStdString(),
                         password.toStdString());
            std::string response = Server::getInstance().responseFromServer(); // Ok or " "
            if (response == "OK")
            {
                deleteLater();
            }
            else
            {
                m_btnReg->setStyleSheet("background-color: red; color: white;");  // Set button to red when declined.
            }
            Server::getInstance().closeSocket();
        }

        else
        {
            m_btnReg->setStyleSheet("background-color: red; color: white;");  // Set button to red when declined.
        }


    });
}

