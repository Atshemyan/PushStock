#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <QValidator>
#include <QRegularExpression>

class EmailValidator : public QValidator
{
public:
    explicit EmailValidator(QObject* parent = nullptr);

    State validate(QString& input, int& pos) const override;
};

#endif // EMAILVALIDATOR_H
