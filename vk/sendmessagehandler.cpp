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

    _uploadAttachments = 0;
}

SendMessageHandler::~SendMessageHandler()
{
    if (_uploadAttachments) delete _uploadAttachments;
}

void SendMessageHandler::send(MessageItem message)
{
    _messageQuery.enqueue(message);

    if (!_isProcessing)
    {
        execSendMessageQuery();
    }
}

void SendMessageHandler::createUploadAttach()
{
    if (!_uploadAttachments)
    {
        _uploadAttachments = new UploadAttachments(_connection);
        connect(_uploadAttachments, SIGNAL(finished()), this, SLOT(sendMessage()));
        connect(_uploadAttachments, SIGNAL(error()), this, SLOT(uploadAttachmentError()));
    }
}

void SendMessageHandler::deleteUploadAttach()
{
    if (_uploadAttachments)
    {
        disconnect(_uploadAttachments, SIGNAL(finished()), this, SLOT(sendMessage()));
        disconnect(_uploadAttachments, SIGNAL(error()), this, SLOT(uploadAttachmentError()));
        _uploadAttachments->deleteLater();
        _uploadAttachments = 0;
    }
}

void SendMessageHandler::execSendMessageQuery()
{
    if (_messageQuery.count() == 0)
    {
        _isProcessing = false;
        return;
    }

    _isProcessing = true;

    MessageItem message = _messageQuery.head();

    if (message->isDeleted())
    {
        _messageQuery.removeFirst();
        execSendMessageQuery();
        return;
    }

    connect(message.data(), SIGNAL(deleted(int)), SLOT(onMessageDeleted(int)));

    if (message->attachments() && message->attachments()->count() > 0)
    {
        createUploadAttach();
        _uploadAttachments->setAttachments(message->id(), message->attachments());
        _uploadAttachments->upload();

        return;
    }

    sendMessage();
}

void SendMessageHandler::sendMessage()
{
    MessageItem message = _messageQuery.dequeue();
    disconnect(message.data(), SIGNAL(deleted(int)), this, SLOT(onMessageDeleted(int)));

    int internalMid = message->id();
    _messagesInProcessing[internalMid] = message;

    Packet *packet = new Packet("messages.send");
    packet->addParam("message", message->plainBody());

    if (message->isGroupChat())
    {
        packet->addParam("chat_id", message->chatId());
    }
    else
    {
        packet->addParam("user_id", message->uid());
    }

    if (message->attachments())
    {
        QStringList attachments;

        for (int i = 0; i < message->attachments()->count(); i++)
        {
            AttachmentItem attachment = message->attachments()->at(i);

            switch (attachment->attachmentType()) {
            case Attachment::Fwd_messages:
            {
                FwdMsgItem fwdMsg = qobject_cast<FwdMsgItem>(attachment);
                packet->addParam("forward_messages", fwdMsg->toString());
                break;
            }

            default:
            {
                attachments.append(attachment->toString());
                break;
            }
            }
        }

        if (attachments.count() > 0)
        {
            packet->addParam("attachment", attachments.join(','));
        }
    }

    packet->addParam("guid", QString::number(message->date().toMSecsSinceEpoch()));
    packet->setId(internalMid);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(sendMessageFinished(const Packet*,QVariantMap)));
    connect(packet, SIGNAL(error(const Packet*,const ErrorResponse*)), this, SLOT(sendMessageError(const Packet*,const ErrorResponse*)));
    emit sending(internalMid);
    _connection->appendQuery(packet);

    _isProcessing = false;
    execSendMessageQuery();
}

void SendMessageHandler::onMessageDeleted(const int id)
{
    if (_uploadAttachments->id() == id)
    {
        deleteUploadAttach();

        _messageQuery.removeFirst();
        _isProcessing = false;
        execSendMessageQuery();
    }
}

void SendMessageHandler::sendMessageFinished(const Packet *sender, const QVariantMap &result)
{
    int response = result.value("response").toInt();

    int internalMid = sender->id();
    int serverMid = response;

    MessageItem message = _messagesInProcessing.take(internalMid);
    message->setId(serverMid);
    message->setIsFake(false);

    emit successfullyMessageSent(internalMid, serverMid);
    delete sender;
}

void SendMessageHandler::sendMessageError(const Packet *sender, const ErrorResponse *errorResponse)
{
    int internalMid = sender->id();

    MessageItem message = _messagesInProcessing.take(internalMid);
    message->setIsError(true);

    emit unsuccessfullyMessageSent(internalMid);
    delete sender;
}

void SendMessageHandler::uploadAttachmentError()
{
    MessageItem message = _messageQuery.dequeue();
    disconnect(message.data(), SIGNAL(deleted(int)), this, SLOT(onMessageDeleted(int)));
    message->setIsError(true);

    emit unsuccessfullyMessageSent(message->id());

    _isProcessing = false;
    execSendMessageQuery();
}

