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

#include "sendmessagehandler.h"

SendMessageHandler::SendMessageHandler(Connection *connection)
{
    _connection = connection;
    _isProcessing = false;
}

void SendMessageHandler::send(MessageItem message)
{
    _messageQuery.enqueue(message);

    if (!_isProcessing)
    {
        execSendMessageQuery();
    }
}

void SendMessageHandler::execSendMessageQuery()
{
    if (_messageQuery.count() == 0)
    {
        return;
    }

    _isProcessing = true;

    //todo добавить обработку аттачей
//    MessageItem message = _messageQuery.head();

//    if (message.attachments && message.attachments.count() > 0)
//    {
//        _uploadAttachments.setAttachments(message.attachments);
//        _uploadAttachments.upload();
//        return;
//    }

    sendMessage();
}

void SendMessageHandler::sendMessage()
{
    MessageItem message = _messageQuery.dequeue();
    int internalMid = message->id();
    _messagesInProcessing[internalMid] = message;

    Packet *packet = new Packet("messages.send");
    packet->addParam("message", message->body());

    if (message->isGroupChat())
    {
        packet->addParam("chat_id", message->chatId());
    }
    else
    {
        packet->addParam("user_id", message->uid());
    }

//    //todo добавить обработку аттачей
//    if (message.attachments)
//    {

//    }

    packet->setId(internalMid);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(sendMessageFinished(const Packet*,QVariantMap)));
    emit sending(internalMid);
    _connection->appendQuery(packet);

    _isProcessing = false;
    execSendMessageQuery();
}

void SendMessageHandler::sendMessageFinished(const Packet *sender, const QVariantMap &result)
{
    int response = result.value("response").toInt();

    int internalMid = sender->id();
    int serverMid = response;

    MessageItem message = _messagesInProcessing.take(internalMid);
    message->setId(serverMid);
    message->setDeliveryReport(true);

    emit successfullyMessageSent(internalMid, serverMid);
    delete sender;
}