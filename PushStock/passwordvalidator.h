#ifndef PASSWORDVALIDATOR_H
#define PASSWORDVALIDATOR_H

#include <QValidator>
#include <QRegularExpression>


class PasswordValidator : public QValidator
{
public:
    explicit PasswordValidator(QObject* parent = nullptr);

    State validate(QString& input, int& pos) const override;
};

#endif // PASSWORDVALIDATOR_H
