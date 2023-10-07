#ifndef DEPOSITPAGE_H
#define DEPOSITPAGE_H
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
#include "server.h"

class DepositPage : public QWidget
{
public:
    DepositPage(QWidget *parent = nullptr, const std::string& username = "NONE");
private:
    void doDeposit(const std::string&);
    QPushButton *m_btnBack {};
    QLineEdit *m_txtMoney {};
    QPushButton *m_btnDeposit {};
    std::string m_currentUserName;



};

#endif // DEPOSITPAGE_H
