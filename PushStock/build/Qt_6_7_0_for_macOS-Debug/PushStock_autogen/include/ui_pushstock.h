/********************************************************************************
** Form generated from reading UI file 'pushstock.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUSHSTOCK_H
#define UI_PUSHSTOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PushStock
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PushStock)
    {
        if (PushStock->objectName().isEmpty())
            PushStock->setObjectName("PushStock");
        PushStock->resize(800, 600);
        centralwidget = new QWidget(PushStock);
        centralwidget->setObjectName("centralwidget");
        PushStock->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PushStock);
        menubar->setObjectName("menubar");
        PushStock->setMenuBar(menubar);
        statusbar = new QStatusBar(PushStock);
        statusbar->setObjectName("statusbar");
        PushStock->setStatusBar(statusbar);

        retranslateUi(PushStock);

        QMetaObject::connectSlotsByName(PushStock);
    } // setupUi

    void retranslateUi(QMainWindow *PushStock)
    {
        PushStock->setWindowTitle(QCoreApplication::translate("PushStock", "PushStock", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PushStock: public Ui_PushStock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHSTOCK_H
