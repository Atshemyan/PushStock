#include "emailvalidator.h"
#include <QRegularExpression>

EmailValidator::EmailValidator(QObject* parent) : QValidator(parent) {}

QValidator::State EmailValidator::validate(QString& input, int& pos) const
{
    QRegularExpression rx("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
    if (rx.match(input).hasMatch())
    {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
