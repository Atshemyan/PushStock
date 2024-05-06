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
#include "mainwindow.h"
#include "loginpage.h"
#include "registrationpage.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setFixedSize(2560, 1664);
    mainWindow.showMaximized();
    return app.exec();
}
