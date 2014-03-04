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

#include "chat.h"

int Chat::_internalMessageId = 0;

Chat::Chat()
{
    _countUnsent = 0;
    _outAttachments = 0;

    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
    connect(_sendMessageHandler, SIGNAL(sending(int)), this, SLOT(onMessageSending(int)));
    connect(_sendMessageHandler, SIGNAL(successfullyMessageSent(int,int)), this, SLOT(onSuccessfullyMessageSent(int,int)));
}

Chat::Chat(const DialogItem dialog)
{
    _dialog = dialog;
    _model = new ChatModel(_dialog, Client::instance()->profile(), this);
    _countUnsent = 0;
    _outAttachments = 0;

    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
    connect(_sendMessageHandler, SIGNAL(sending(int)), this, SLOT(onMessageSending(int)));
    connect(_sendMessageHandler, SIGNAL(successfullyMessageSent(int,int)), this, SLOT(onSuccessfullyMessageSent(int,int)));
}

Chat::~Chat()
{
    if (_outAttachments) delete _outAttachments;

    delete _sendMessageHandler;
    delete _model;
}

int Chat::id() const
{
    return _dialog->id();
}

bool Chat::isGroupChat() const
{
    return _dialog->isGroupChat();
}

DialogItem Chat::dialog() const
{
    return _dialog;
}

ChatModel *Chat::model() const
{
    return _model;
}

int Chat::countUnsent() const
{
    return _countUnsent;
}

void Chat::addTempMessageQueue(MessageItem message)
{
    _tempMessageQueue.append(message);
}

void Chat::sendMessage(const QString &text)
{
    if (text.isEmpty() && (!_outAttachments || !_outAttachments->count()))
    {
        return;
    }

    MessageItem message = MessageItem::create();
    message->setId(--_internalMessageId);

    if (isGroupChat())
    {
        message->setChatId(_dialog->groupChatHandler()->chatId());
        message->setUid(Client::instance()->uid());
    }
    else
    {
        message->setUid(_dialog->profile()->id());
    }

    message->setDate(QDateTime::currentDateTime());
    message->setIsUnread(true);
    message->setIsOut(true);
    message->setIsError(false);
    message->setBody(text);

    message->setAttachments(_outAttachments);
    _outAttachments = 0;
    _model->prepend(message);
    _sendMessageHandler->send(message);
}

void Chat::addAttachments(const QList<QUrl> &list)
{
    if (!_outAttachments)
    {
        _outAttachments = new AttachmentList();
    }

    for (int i = 0; i < list.count(); i++)
    {
        PhotoItem photo = PhotoItem::create();
        photo->setSrc(list.at(i));
        photo->setSrcBig(list.at(i));

        _outAttachments->add(photo);
    }
}

void Chat::onMessageSending(const int internalMid)
{
    _countUnsent++;
}

void Chat::onSuccessfullyMessageSent(const int internalMid, const int serverMid)
{
    _countUnsent--;

    if (countUnsent() == 0 && _tempMessageQueue.count())
    {
        for (int i = 0; i < _tempMessageQueue.count(); i++)
        {
            _model->prepend(_tempMessageQueue.at(i), true);
        }

        _tempMessageQueue.clear();
    }
}
