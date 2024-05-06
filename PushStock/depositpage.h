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
#include "depositvalidator.h"
#include "server.h"

class DepositPage : public QWidget
{
public:
    DepositPage(QWidget *parent = nullptr, const std::string& username = "NONE", uint languageIndex = 0);
private:
    void doDeposit(const std::string&);
    QPushButton *m_btnBack {};
    QLineEdit *m_txtMoney {};
    QPushButton *m_btnDeposit {};
    uint m_languageIndex = 0;
    QSize m_size = parentWidget()->size();
    std::string m_currentUserName;
    void pageHeader();
    void pageLogo();
    void depositOpportunity();

};

#endif // DEPOSITPAGE_H
