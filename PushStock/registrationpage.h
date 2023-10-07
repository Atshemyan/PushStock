#ifndef REGISTRATIONPAGE_H
#define REGISTRATIONPAGE_H
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

class RegistrationPage : public QWidget
{
public:
    RegistrationPage(QWidget *parent = nullptr);
private:
    QLineEdit *m_txtName {};
    QLineEdit *m_txtSurname {};
    QLineEdit *m_txtPhone {};
    QLineEdit *m_txtUser {};
    QLineEdit *m_txtPassword {};
    QPushButton *m_btnBack {};
    QPushButton *m_btnReg {};

};

#endif // REGISTRATIONPAGE_H
