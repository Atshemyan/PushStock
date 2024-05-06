#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setWindowTitle("PushStock");
    this->setDarkTheme();
    this->initButtons();
    this->initLabels();
    this->initLayouts();
    this->connectButtons();
    this->initMainPage();

}

void MainWindow::connectButtons()
{
    QObject::connect(m_btnLogin, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    QObject::connect(m_btnRegister, &QPushButton::clicked, this, &MainWindow::showRegistrationPage);
}

void MainWindow::initMainPage()
{
    QWidget *mainPage = new QWidget;
    m_mainLayout->addLayout(m_header);
    m_mainLayout->addLayout(m_bodyLayout);

    mainPage->setLayout(m_mainLayout);
    setCentralWidget(mainPage);
}

void MainWindow::initLayouts()
{
    m_buttonLayout = new QHBoxLayout;
    m_mainLayout = new QVBoxLayout;
    m_bodyLayout = new QVBoxLayout;
    m_header = new QHBoxLayout;

    m_buttonLayout->addWidget(m_btnLogin);
    m_buttonLayout->addWidget(m_btnRegister);
    m_languageSelection = new QComboBox;
    m_languageSelection->addItem("Armenian");
    m_languageSelection->addItem("English");
    m_languageSelection->setFixedWidth(150);
    QObject::connect(m_languageSelection, &QComboBox::currentIndexChanged, [this]()
    {
        if (this->m_languageSelection->currentIndex() == 0) {
            m_btnLogin->setText("Մուտք");
            m_btnRegister->setText("Գրանցվել");
        }
        else {
            m_btnLogin->setText("Login");
            m_btnRegister->setText("Register");
        }

    });
    m_header->addWidget(m_languageSelection);
    m_header->setAlignment(Qt::AlignLeft);

    m_bodyLayout->addWidget(m_imageLabel, 0, Qt::AlignHCenter);
    m_bodyLayout->addLayout(m_buttonLayout);

    m_bodyLayout->setContentsMargins(200, 20, 200, 20);
    m_bodyLayout->setSpacing(20);
    m_bodyLayout->setAlignment(Qt::AlignCenter);
}

void MainWindow::initLabels()
{
    m_imageLabel = new QLabel;
    QPixmap image(":images/logo.png");
    m_imageLabel->setPixmap(image);
}

void MainWindow::initButtons()
{
    m_btnLogin = new QPushButton("Մուտք");
    m_btnLogin->setStyleSheet("QPushButton { background: transparent; color: white; }"
                              "QPushButton:hover { color: rgb(241, 180, 41); }");
    m_btnRegister = new QPushButton("Գրանցվել");
    m_btnRegister->setStyleSheet("QPushButton { background: transparent; color: white; }"
                                 "QPushButton:hover { color: rgb(241, 180, 41); }");
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
    m_loginPage = new LoginPage(this, m_languageSelection->currentIndex());
    m_loginPage->show();
}

void MainWindow::showRegistrationPage()
{
    m_registrationPage = new RegistrationPage(this, m_languageSelection->currentIndex());
    m_registrationPage->show();
}

