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
    QQueue<int> _ids;

protected slots:
    void onLongPollMessageInAdded(const int id, const MessageItem message, const ProfileItem profile);
    void onLongPollMessageOutAdded(const int id, const MessageItem message, const ProfileItem profile);
    void onMessageFlagsSet(const int mid, const int mask, const int id);
    void onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date);
    void onObsoleteFriendsOnline();
    void onRebuild();
};

#endif // CHATSHANDLER_H
