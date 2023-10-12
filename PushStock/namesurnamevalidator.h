#ifndef NAMESURNAMEVALIDATOR_H
#define NAMESURNAMEVALIDATOR_H

#include <QValidator>
#include <QRegularExpression>

class NameSurnameValidator : public QValidator
{
public:
    explicit NameSurnameValidator(QObject* parent = nullptr);

    State validate(QString& input, int& pos) const override;
};

#endif // NAMESURNAMEVALIDATOR_H
