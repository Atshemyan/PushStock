#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include "server.h"
#include <QComboBox>

class MessageBuilder
{
public:
    virtual void sendCommandToServer() = 0;
};

#endif // MESSAGEBUILDER_H
