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

#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QSharedPointer>
#include "observablecollection.h"
#include "messagebase.h"
#include "messageitem.h"

class MessageListPrivate : public ObservableCollection<MessageBaseItem>
{
    Q_OBJECT

public:
    MessageListPrivate();
    int notFakeMsgCount() const;
};

typedef QSharedPointer<MessageListPrivate> MessageList;

#endif // MESSAGELIST_H
