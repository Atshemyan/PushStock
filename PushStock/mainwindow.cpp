#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setWindowTitle("PushStock");
    setGeometry(100, 100, 1920, 1080);

    // Initialize member variables
    m_btnLogin = new QPushButton("Login");
    m_btnLogin->setStyleSheet("QPushButton { background: transparent; color: white; }"
                             "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnRegister = new QPushButton("Register");
    m_btnRegister->setStyleSheet("QPushButton { background: transparent; color: white; }"
                             "QPushButton:hover { color: rgb(241, 180, 41); }");

    m_imageLabel = new QLabel;
    m_buttonLayout = new QHBoxLayout();
    m_mainLayout = new QVBoxLayout();
    QWidget *registrationPage = new QWidget;

    // Load an image and set it in the QLabel
    QPixmap image("/Users/hayk/Desktop/1.png"); //maybe from server
    m_imageLabel->setPixmap(image);

    m_buttonLayout->addWidget(m_btnLogin);
    m_buttonLayout->addWidget(m_btnRegister);

    m_mainLayout->addWidget(m_imageLabel, 0, Qt::AlignHCenter);
    m_mainLayout->addLayout(m_buttonLayout);

    m_mainLayout->setContentsMargins(200, 20, 200, 20);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setAlignment(Qt::AlignCenter);

    registrationPage->setLayout(m_mainLayout);

    setCentralWidget(registrationPage);

    QObject::connect(m_btnLogin, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    QObject::connect(m_btnRegister, &QPushButton::clicked, this, &MainWindow::showRegistrationPage);
    //inch vor jamanaky mek kanchel isServerEnabled function-y
//    QTimer *timer = new QTimer(this);

//    QObject::connect(timer, &QTimer::timeout, []()
//    {
//        if (Server::getInstance().checkConnection())
//        {
//            qDebug() << "Server is enable \n";
//        }
//        else
//        {
//            qDebug() << "Server is disabled \n";
//        }
//    });

//    timer->start(5000);



}

void MainWindow::setDarkTheme()
{
    this->setStyleSheet(
        "QWidget { background-color: black; color: white; }"
        "QLabel { color: white; }"
        "QLineEdit { background-color: #333; color: white; border: 2px solid #555; border-radius: 5px; }"
        "QPushButton { background-color: #007ACC; color: white; border: none; border-radius: 5px; padding: 10px; }"
        "QPushButton:hover { background-color: #005F9C; }"
        );
}

void MainWindow::showLoginPage()
{
    m_loginPage = new LoginPage(this);
    m_loginPage->show();
}

void MainWindow::showRegistrationPage()
{
    m_registrationPage = new RegistrationPage(this);
    m_registrationPage->show();
}


MainWindow::~MainWindow()
{
    Server::getInstance().closeSocket();
}



