#include "PhoneValidator.h"

PhoneValidator::PhoneValidator(QObject* parent) : QValidator(parent) {}

QValidator::State PhoneValidator::validate(QString& input, int& pos) const
{
    // Define a regular expression pattern for allowed phone number prefixes
    QRegularExpression rx("^(033|099|098|096|095|055|041|097|077|094)\\d{6}$");

    if (rx.match(input).hasMatch())
    {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
