#ifndef COINPRICETHREAD_H
#define COINPRICETHREAD_H
#include <QThread>

class CoinPriceThread : public QThread
{
    Q_OBJECT
public:
    CoinPriceThread(QObject *parent = nullptr);
    void stopThread();
    void continueThread();
signals:
    void updatePrice();

protected:
    void run() override;

private:
    bool m_stopThread;
};

#endif // COINPRICETHREAD_H
