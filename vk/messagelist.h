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

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QVector>
#include "messageitem.h"

class MessageListPrivate : public QObject
{
    Q_OBJECT

public:
    MessageListPrivate();
    ~MessageListPrivate();
    void add(MessageItem message);
    void add(const QList<MessageItem> &messages);
    void prepend(MessageItem message);
    bool replace(const MessageItem item);
    void replace(const int i, const MessageItem item);
    int indexOf(const int mid) const;
    MessageItem at(const int i) const;
    MessageItem item(const int mid) const;
    void removeAt(const int i);
    int count() const;
    QList<MessageItem> toList() const;
    QVector<MessageItem> toVector() const;

private:
    QList<MessageItem> _messages;

protected slots:
    void onItemChanged(const int mid, const QString &propertyName);

signals:
    void itemChanged(const int i);
};

typedef QSharedPointer<MessageListPrivate> MessageList;

#endif // MESSAGELIST_H
