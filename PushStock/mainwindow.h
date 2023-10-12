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
#include "loginpage.h"
#include "registrationpage.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
private:
    QVBoxLayout *m_mainLayout {};
    QHBoxLayout *m_buttonLayout {};
    QPushButton *m_btnLogin {};
    QPushButton *m_btnRegister {};
    QLabel *m_imageLabel {};
    LoginPage *m_loginPage {};
    RegistrationPage *m_registrationPage {};
    void setDarkTheme();
    void showLoginPage();
    void showRegistrationPage();
    void initButtons();
    void initLabels();
    void initLayouts();
    void initMainPage();
    void connectButtons();
};

#endif // MAINWINDOW_H
