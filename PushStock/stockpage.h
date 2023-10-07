#ifndef STOCKPAGE_H
#define STOCKPAGE_H
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
#include <QComboBox>
#include <QTimer>
#include "depositpage.h"

class StockPage : public QWidget
{

public:
    StockPage(QWidget *parent = nullptr, const std::string& username = "NONE");

private:
    QComboBox *m_coinsSelection {};
    QLabel *m_moneyToShow {};
    QPushButton *m_btnDeposit {};
    QPushButton *m_btnBuy {};
    QPushButton *m_btnSell {};
    QLabel *m_nameToShow {};
    DepositPage *m_depositPage {};
    std::string m_currentUserUsername;



};

#endif // STOCKPAGE_H
