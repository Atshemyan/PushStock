#include "pushstock.h"
#include "./ui_pushstock.h"

PushStock::PushStock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PushStock)
{
    ui->setupUi(this);
}

PushStock::~PushStock()
{
    delete ui;
}

