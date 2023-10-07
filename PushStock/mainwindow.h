#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QTimer>
#include "loginpage.h"
#include "registrationpage.h"
#include "server.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    void setDarkTheme();
    void showLoginPage();
    void showRegistrationPage();
    ~MainWindow();



private:
    QVBoxLayout *m_mainLayout {};
    QHBoxLayout *m_buttonLayout {};
    QPushButton *m_btnLogin {};
    QPushButton *m_btnRegister {};
    QLabel *m_imageLabel {};
    LoginPage *m_loginPage {};
    RegistrationPage *m_registrationPage {};
};

#endif // MAINWINDOW_H
