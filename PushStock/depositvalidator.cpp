#include "depositvalidator.h"

DepositValidator::DepositValidator(QObject* parent)
    : QValidator(parent)
{
}

QValidator::State DepositValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos);

    if(input.isEmpty())
    {
        return QValidator::Intermediate; // Если поле пустое, это состояние промежуточное, и вы можете показать предупреждение или сообщение об ошибке.
    }

    QRegularExpression rx(R"(^\d+(\.\d+)?$)"); // Это регулярное выражение соответствует положительным числам с плавающей точкой.

    if (rx.match(input).hasMatch())
    {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}

