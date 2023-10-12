#include "passwordvalidator.h"
#include <QRegularExpression>

PasswordValidator::PasswordValidator(QObject* parent) : QValidator(parent) {}

QValidator::State PasswordValidator::validate(QString& input, int& pos) const
{
    QRegularExpression rx("^[A-Za-z0-9!@#$%^&*()-_+=]{6,}$");
    if (rx.match(input).hasMatch())
    {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
