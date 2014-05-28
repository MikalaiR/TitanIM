/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include <QObject>
#include <QSharedPointer>
#include <QDateTime>
#include "notifypropertybase.h"

class MessageBase : public NotifyPropertyBase
{
    Q_OBJECT
    Q_ENUMS(MessageType)
    Q_PROPERTY(MessageType type READ messageType) //todo

public:
    enum MessageType
    {
        Text,
        Typing,
        Service,
        Unknown = -1
    };

    MessageBase();
    MessageType messageType() const;
    QDateTime date() const;
    void setDate(const QDateTime &date);
    bool deleted() const;
    void setDeleted(const bool deleted);

protected:
    void setMessageType(const MessageType messageType);

protected:
    MessageType _messageType;
    QDateTime _date;
    bool _deleted;
};

typedef QSharedPointer<MessageBase> MessageBaseItem;

#endif // MESSAGEBASE_H
