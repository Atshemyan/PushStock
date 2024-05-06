#include "coinpricethread.h"

CoinPriceThread::CoinPriceThread(QObject *parent)
    : QThread(parent)
{}


void CoinPriceThread::run() {
    while (!m_stopThread) {
        emit updatePrice();
        msleep(1000);
    }
}

void CoinPriceThread::stopThread() {
    m_stopThread = true;
}

void CoinPriceThread::continueThread() {
    m_stopThread = false;
    start();
}


