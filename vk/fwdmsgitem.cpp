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

#include "fwdmsgitem.h"

fwdMsgItemPrivate::fwdMsgItemPrivate()
{
    setAttachmentType(Fwd_messages);
}

QVariantList fwdMsgItemPrivate::messages() const
{
    QVariantList list;

    for (int i = 0; i < _messages->count(); i++)
    {
        list.append(QVariant::fromValue(_messages->at(i).data()));
    }

    return list;
}

void fwdMsgItemPrivate::setMessages(const MessageList messages)
{
    _messages = messages;
}

int fwdMsgItemPrivate::countMessages() const
{
    return _messages->count();
}

QString fwdMsgItemPrivate::toString() const
{
    QStringList list;

    for (int i = 0; i < _messages->count(); i++)
    {
        list.append(QString::number(_messages->at(i)->id()));
    }

    return list.join(',');
}
