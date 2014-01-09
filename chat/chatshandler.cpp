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

    _historyPacket = new HistoryPacket(Client::instance()->connection());
    connect(_historyPacket, SIGNAL(history(const HistoryPacket*,int,MessageList)), this, SLOT(onHistoryLoaded(const HistoryPacket*,int,MessageList)));
}

ChatsHandler::~ChatsHandler()
{
    delete _historyPacket;
    clear();
}

void ChatsHandler::create(const DialogItem dialog)
{
    int id = dialog->id();

    _chats[id] = new Chat(dialog);
    _historyPacket->load(id);
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
        chat(id)->model()->prepend(message);
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
            chat->addTempMessageQueue(message);
        }
        else
        {
            chat->model()->prepend(message, true);
        }
    }
}

void ChatsHandler::onHistoryLoaded(const HistoryPacket *sender, const int id, const MessageList &messages)
{
    if (contains(id))
    {
        chat(id)->model()->replaceAll(messages);
    }
}
