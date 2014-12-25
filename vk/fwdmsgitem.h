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

#ifndef FWDMSGITEM_H
#define FWDMSGITEM_H

#include <QObject>
#include <QQmlEngine>
#include "attachmentitem.h"
#include "messagelist.h"
#include "profilelist.h"
#include "client.h"

class fwdMsgItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QVariantList messages READ messages CONSTANT) //todo

public:
    fwdMsgItemPrivate();
    QVariantList messages() const;
    void setMessages(const MessageList messages);
    Q_INVOKABLE QVariant getProfile(const int id) const;

private:
    MessageList _messages;
    static ProfileList _users;
};

typedef QSharedPointer<fwdMsgItemPrivate> FwdMsgItem;

#endif // FWDMSGITEM_H
