#ifndef PHONEVALIDATOR_H
#define PHONEVALIDATOR_H

#include <QValidator>
#include <QRegularExpression>


class PhoneValidator : public QValidator
{
public:
    explicit PhoneValidator(QObject* parent = nullptr);

    State validate(QString& input, int& pos) const override;
};

#endif // PHONEVALIDATOR_H
