#ifndef PUSHSTOCK_H
#define PUSHSTOCK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PushStock; }
QT_END_NAMESPACE

class PushStock : public QMainWindow
{
    Q_OBJECT

public:
    PushStock(QWidget *parent = nullptr);
    ~PushStock();

private:
    Ui::PushStock *ui;
};
#endif // PUSHSTOCK_H
