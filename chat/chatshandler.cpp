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
    connect(Client::instance()->longPoll(), SIGNAL(messageInAdded(DialogItem)), this, SLOT(onLongPollMessageInAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(messageOutAdded(DialogItem)), this, SLOT(onLongPollMessageOutAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(messageFlagsReseted(int,int,int,uint)), this, SLOT(onMessageFlagsReseted(int,int,int,uint)));
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
    _chats.clear();
    //todo память
}

void ChatsHandler::onLongPollMessageInAdded(const DialogItem dialog)
{
    int id = dialog->id();

    if (contains(id))
    {
        MessageItem message = dialog->message();
        chat(id)->addInMessage(message);
    }
}

void ChatsHandler::onLongPollMessageOutAdded(const DialogItem dialog)
{
    int id = dialog->id();

    if (contains(id))
    {
        Chat *chat = this->chat(id);
        MessageItem message = dialog->message();

        message->setDeliveryReport(true);

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
