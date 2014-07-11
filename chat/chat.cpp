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
    connect(_sendMessageHandler, SIGNAL(unsuccessfullyMessageSent(int)), this, SLOT(onUnsuccessfullyMessageSent(int)));
}

Chat::Chat(const DialogItem dialog)
{
    _dialog = dialog;
    connect(_dialog.data(), SIGNAL(newTyping(TypingItem)), this, SLOT(addTyping(TypingItem)));

    _model = new ChatModel(_dialog, this);
    connect(_model, SIGNAL(rowsAllReplaced()), this, SLOT(onModelRowsAllReplaced()));

    _countUnsent = 0;
    _outAttachments = 0;

    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
    connect(_sendMessageHandler, SIGNAL(sending(int)), this, SLOT(onMessageSending(int)));
    connect(_sendMessageHandler, SIGNAL(successfullyMessageSent(int,int)), this, SLOT(onSuccessfullyMessageSent(int,int)));
    connect(_sendMessageHandler, SIGNAL(unsuccessfullyMessageSent(int)), this, SLOT(onUnsuccessfullyMessageSent(int)));
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

void Chat::addInMessage(const MessageItem message)
{
    _model->prepend(message);
}

void Chat::addOutMessage(const MessageItem message)
{
    _model->prepend(message, true);
}

void Chat::addTempOutMessageQueue(MessageItem message)
{
    _tempOutMessageQueue.append(message);
}

void Chat::addTyping(const TypingItem typing)
{
    typing->setId(--_internalMessageId);
    _model->prepend(typing);
}

void Chat::addTyping(const QList<TypingItem> &typingList)
{
    for (int i = 0; i < typingList.count(); i++)
    {
        addTyping(typingList.at(i));
    }
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

    message->setDate(Utils::currentDateTime());
    message->setIsUnread(true);
    message->setIsOut(true);
    message->setBody(text);

    message->setAttachments(_outAttachments);
    _outAttachments = 0;

    _model->prepend(message);
//    _dialog->setMessage(message); //todo

    _sendMessageHandler->send(message);
}

void Chat::sendTyping()
{
    Packet *packet = new Packet("messages.setActivity");
    packet->addParam("user_id", _dialog->id());
    packet->addParam("type", "typing");
    Client::instance()->connection()->appendQuery(packet);
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

void Chat::markAsRead()
{
    Packet *packet = new Packet("messages.markAsRead");
    packet->addParam("user_id", _dialog->id());
    Client::instance()->connection()->appendQuery(packet);
}

void Chat::onMessageSending(const int internalMid)
{
    _countUnsent++;
}

void Chat::onSuccessfullyMessageSent(const int internalMid, const int serverMid)
{
    _countUnsent--;

    if (countUnsent() == 0 && _tempOutMessageQueue.count())
    {
        for (int i = 0; i < _tempOutMessageQueue.count(); i++)
        {
            _model->prepend(_tempOutMessageQueue.at(i), true);
        }

        _tempOutMessageQueue.clear();
    }
}

void Chat::onUnsuccessfullyMessageSent(const int internalMid)
{
    _countUnsent--;

    if (countUnsent() == 0 && _tempOutMessageQueue.count())
    {
        for (int i = 0; i < _tempOutMessageQueue.count(); i++)
        {
            _model->prepend(_tempOutMessageQueue.at(i), true);
        }

        _tempOutMessageQueue.clear();
    }
}

void Chat::onModelRowsAllReplaced()
{
    if (_dialog->typingHandler())
    {
        addTyping(_dialog->typingHandler()->toList());
    }
}
