#ifndef LOGINPAGE_H
#define LOGINPAGE_H
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
#include "emailvalidator.h"
#include "passwordvalidator.h"
#include "stockpage.h"
#include "loginbuilder.h"


class LoginPage : public QWidget
{
public:
    LoginPage(QWidget* parent = nullptr);
private:
    QPushButton *m_btnBack {};
    QLineEdit *m_txtUser {};
    QLineEdit *m_txtPassword {};
    QPushButton *m_btnLogin {};
    StockPage *m_stockPage {};
    QLabel *m_topLabel {};
    QLabel *m_logoLabel {};
    QSize m_parentSize = parentWidget()->size();

    void pageHeader();
    void pageLogo();
    void pageLogin();
    void loginButtonConnect();

};

#endif // LOGINPAGE_H
