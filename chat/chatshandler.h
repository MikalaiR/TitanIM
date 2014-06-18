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

#ifndef CHATSHANDLER_H
#define CHATSHANDLER_H

#include <QObject>
#include <QMap>
#include "chat.h"
#include "vk/client.h"

class ChatsHandler : public QObject
{
    Q_OBJECT

public:
    ChatsHandler();
    ~ChatsHandler();
    void create(const DialogItem dialog);
    bool contains(const int id) const;
    Chat* chat(const int id) const;
    void clear();

private:
    QMap<int, Chat*> _chats;

protected slots:
    void onLongPollMessageInAdded(const int id, const MessageItem message);
    void onLongPollMessageOutAdded(const int id, const MessageItem message);
    void onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date);
};

#endif // CHATSHANDLER_H
