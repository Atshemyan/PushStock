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
#include "registerbuilder.h"
#include "namesurnamevalidator.h"
#include "emailvalidator.h"
#include "passwordvalidator.h"
#include "phonevalidator.h"

class RegistrationPage : public QWidget
{
public:
    RegistrationPage(QWidget *parent = nullptr, uint languageIndex = 0);
private:
    QLineEdit *m_txtName {};
    QLineEdit *m_txtSurname {};
    QLineEdit *m_txtPhone {};
    QLineEdit *m_txtUser {};
    QLineEdit *m_txtPassword {};
    QPushButton *m_btnBack {};
    QPushButton *m_btnReg {};
    QSize m_size = parentWidget()->size();
    uint m_languageIndex = 0;

    void pageHeader();
    void pageLogo();
    void pageRegister();
    void registerButtonConnect();

};

#endif // REGISTRATIONPAGE_H
