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

#include "messagelist.h"

MessageListPrivate::MessageListPrivate()
{
}

MessageListPrivate::~MessageListPrivate()
{
}

void MessageListPrivate::add(MessageItem message)
{
    _messages.append(message);
    connect(message.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

void MessageListPrivate::add(const QList<MessageItem> &messages)
{
    for (int i = 0; i < messages.count(); i++)
    {
        add(messages.at(i));
    }
}

void MessageListPrivate::prepend(MessageItem message)
{
    _messages.prepend(message);
    connect(message.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

bool MessageListPrivate::replace(const MessageItem item)
{
    int i = indexOf(item->mid());

    if (i > -1)
    {
        replace(i, item);
        return true;
    }

    return false;
}

void MessageListPrivate::replace(const int i, const MessageItem item)
{
    disconnect(_messages.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _messages.replace(i, item);
    connect(item.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));

    emit itemChanged(i);
}

int MessageListPrivate::indexOf(const int mid) const
{
    for (int i = 0; i < _messages.count(); i++)
    {
        if (_messages.at(i)->mid() == mid)
            return i;
    }

    return -1;
}

MessageItem MessageListPrivate::at(const int i) const
{
    return _messages.at(i);
}

MessageItem MessageListPrivate::item(const int mid) const
{
    foreach (MessageItem item, _messages)
    {
        if (item->mid() == mid)
            return item;
    }

    return MessageItem();
}

void MessageListPrivate::removeAt(const int i)
{
    disconnect(_messages.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _messages.removeAt(i);
}

int MessageListPrivate::count() const
{
    return _messages.count();
}

QList<MessageItem> MessageListPrivate::toList() const
{
    return _messages;
}

QVector<MessageItem> MessageListPrivate::toVector() const
{
    return _messages.toVector();
}

void MessageListPrivate::onItemChanged(const int mid, const QString &propertyName)
{
    emit itemChanged(indexOf(mid));
}
