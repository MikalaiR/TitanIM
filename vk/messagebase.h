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
#include "utils.h"

class MessageBase : public NotifyPropertyBase
{
    Q_OBJECT
    Q_ENUMS(MessageType)
    Q_PROPERTY(MessageType type READ messageType) //todo
    Q_PROPERTY(QDateTime date READ date CONSTANT)

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
    bool isFake() const;
    void setIsFake(const bool isFake);
    int uid() const;
    void setUid(const int uid);
    QDateTime date() const;
    void setDate(const QDateTime &date);
    bool isDeleted() const;
    void setDeleted(const bool isDeleted);
    bool isChecked() const;
    void setChecked(const bool isChecked);

protected:
    void setMessageType(const MessageType messageType);

protected:
    MessageType _messageType;
    bool _isFake;
    int _uid;
    QDateTime _date;
    bool _isDeleted;
    bool _isChecked;

signals:
    void deleted(const int id);
};

typedef QSharedPointer<MessageBase> MessageBaseItem;

#endif // MESSAGEBASE_H
