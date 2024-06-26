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
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QListWidget>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QScrollBar>
#include <QSet>
#include <string>
#include "depositpage.h"
#include "server.h"
#include "coinpricethread.h"

class StockPage : public QWidget
{

public:
    StockPage(QWidget *parent = nullptr, const std::string& username = "NONE", uint languageIndex = 0);
private:
    QComboBox *m_coinsSelection {};
    uint m_languageIndex = 0;
    QLabel *m_moneyToShow {};
    QPushButton *m_btnDeposit {};
    QPushButton *m_btnExit {};
    QPushButton *m_btnBuy {};
    QInputDialog *m_dialogBuy {};
    QPushButton *m_btnSell {};
    QLabel *m_nameToShow {};
    QLabel *m_coinsCount {};
    DepositPage *m_depositPage {};
    QChart *m_priceChart {};
    QChartView *m_priceChartView {};
    QLineSeries *m_priceSeries {};
    std::string m_currentUserUsername;
    QLabel *m_topLabel {};
    QLabel *m_botLabel {};
    QListWidget *m_transactionListWidget {};
    QLabel *m_stockLabel {};
    QLabel *m_coinPrice {};
    QSet<QString> m_existingItemsInList;
    QSize m_size {};
    CoinPriceThread *m_priceThread {new CoinPriceThread};
    int m_maxValueOfChart = 0;
    int m_minValueOfChart = 0;
    bool m_firstTime = true;
    double m_prevPriceOfCoin {};
    void removeDots(std::string& response);
    void updateChart(std::string& response);

    void resetChart();
    void initDepositButton();
    void initExitButton();
    void initTopLabel();
    void initBotLabel();
    void initBotButtons();
    void updateTransactionList();
    void initCoinPrice();
    void priceGraphic();
    void moneyShow();
    void initTransactionList();
    void getCoinsCount();
    void coinShow();
    void settingCoinPrice();
    void coinSelection();
    void createEmptyChart();
    void initStockLabel();

};

#endif // STOCKPAGE_H
