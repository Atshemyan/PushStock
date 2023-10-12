#ifndef DEPOSITVALIDATOR_H
#define DEPOSITVALIDATOR_H
#include <QValidator>
#include <QRegularExpression>



class DepositValidator : public QValidator
{
public:
    explicit DepositValidator(QObject* parent = nullptr);

    QValidator::State validate(QString& input, int& pos) const override;
};

#endif // DEPOSITVALIDATOR_H
