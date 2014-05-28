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

#include "messagebase.h"

MessageBase::MessageBase()
{
    _messageType = Unknown;
    _deleted = false;
}

MessageBase::MessageType MessageBase::messageType() const
{
    return _messageType;
}

QDateTime MessageBase::date() const
{
    return _date;
}

void MessageBase::setDate(const QDateTime &date)
{
    if (_date != date)
    {
        _date = date;
        emitPropertyChanged("date");
    }
}

bool MessageBase::deleted() const
{
    return _deleted;
}

void MessageBase::setDeleted(const bool deleted)
{
    if (_deleted != deleted)
    {
        _deleted = deleted;
        emitPropertyChanged("deleted");
    }
}

void MessageBase::setMessageType(const MessageBase::MessageType messageType)
{
    if (_messageType != messageType)
    {
        _messageType = messageType;
        emitPropertyChanged("messageType");
    }
}
