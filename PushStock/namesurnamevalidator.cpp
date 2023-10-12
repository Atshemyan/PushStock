#include "namesurnamevalidator.h"

NameSurnameValidator::NameSurnameValidator(QObject* parent) : QValidator(parent) {}

QValidator::State NameSurnameValidator::validate(QString& input, int& pos) const
{
    QRegularExpression rx("^[A-Za-z\\s]*$");
    if (rx.match(input).hasMatch() && !input.isEmpty())
    {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
