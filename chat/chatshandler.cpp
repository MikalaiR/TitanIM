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

#include "chatshandler.h"

ChatsHandler::ChatsHandler()
{
    LongPoll *longPoll = Client::instance()->longPoll();

    connect(longPoll, SIGNAL(messageInAdded(int,MessageItem,ProfileItem)), this, SLOT(onLongPollMessageInAdded(int,MessageItem,ProfileItem)));
    connect(longPoll, SIGNAL(messageOutAdded(int,MessageItem,ProfileItem)), this, SLOT(onLongPollMessageOutAdded(int,MessageItem,ProfileItem)));
    connect(longPoll, SIGNAL(messageFlagsReseted(int,int,int,uint)), this, SLOT(onMessageFlagsReseted(int,int,int,uint)));
}

ChatsHandler::~ChatsHandler()
{
    clear();
}

void ChatsHandler::create(const DialogItem dialog)
{
    int id = dialog->id();

    Chat *chat = new Chat(dialog);
    _chats[id] = chat;

    chat->model()->load();
}

bool ChatsHandler::contains(const int id) const
{
    return _chats.contains(id);
}

Chat *ChatsHandler::chat(const int id) const
{
    return _chats.value(id);
}

void ChatsHandler::clear()
{
    QMapIterator<int, Chat*> i(_chats);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }

    _chats.clear();
}

void ChatsHandler::onLongPollMessageInAdded(const int id, const MessageItem message, const ProfileItem profile)
{
    Q_UNUSED(profile);

    if (contains(id))
    {
        if (message->actionMid() > 0)
        {
            message->getAllFields(Client::instance()->connection());
        }

        chat(id)->addInMessage(message);
    }
}

void ChatsHandler::onLongPollMessageOutAdded(const int id, const MessageItem message, const ProfileItem profile)
{
    Q_UNUSED(profile);

    if (contains(id))
    {
        if (message->actionMid() > 0)
        {
            message->getAllFields(Client::instance()->connection());
        }

        Chat *chat = this->chat(id);

        if (chat->countUnsent() > 0)
        {
            chat->addTempOutMessageQueue(message);
        }
        else
        {
            chat->addOutMessage(message);
        }
    }
}

void ChatsHandler::onMessageFlagsReseted(const int mid, const int mask, const int id, const uint date)
{
    if (mask & LongPoll::Unread)
    {
        if (contains(id))
        {
            chat(id)->model()->markAsRead(mid);
        }
    }
}
