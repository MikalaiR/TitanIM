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

#include "messageitem.h"
#include "messageparser.h"

MessageItemPrivate::MessageItemPrivate()
{
    setMessageType(Text);

    _uid = 0;
    _isError = false;
    _deliveryReport = false;
    _chatId = 0;
    _emoji = false;
    _attachments = 0;
    _isLoading = false;
}

MessageItemPrivate::~MessageItemPrivate()
{
    if (_attachments) delete _attachments;
}

int MessageItemPrivate::uid() const
{
    return _uid;
}

void MessageItemPrivate::setUid(const int uid)
{
    if (_uid != uid)
    {
        _uid = uid;
        emitPropertyChanged("uid");
    }
}

int MessageItemPrivate::fromId() const
{
    return isGroupChat() ? _chatId + GROUP_CHAT_OFFSET : _uid;
}

void MessageItemPrivate::setFromId(const int fromId)
{
    if (fromId > GROUP_CHAT_OFFSET)
    {
        setChatId(fromId - GROUP_CHAT_OFFSET);
    }
    else
    {
        setUid(fromId);
    }
}

uint MessageItemPrivate::unixtime() const
{
    return _date.toTime_t();
}

bool MessageItemPrivate::isUnread() const
{
    return _isUnread;
}

void MessageItemPrivate::setIsUnread(const bool isUnread)
{
    if (_isUnread != isUnread)
    {
        _isUnread = isUnread;
        emitPropertyChanged("isUnread");
    }
}

bool MessageItemPrivate::isOut() const
{
    return _isOut;
}

void MessageItemPrivate::setIsOut(const bool isOut)
{
    if (_isOut != isOut)
    {
        _isOut = isOut;
        emitPropertyChanged("isOut");
    }
}

bool MessageItemPrivate::isError() const
{
    return _isError;
}

void MessageItemPrivate::setIsError(const bool isError)
{
    if (_isError != isError)
    {
        _isError = isError;
        emitPropertyChanged("isError");
    }
}

bool MessageItemPrivate::isLoading() const
{
    return _isLoading;
}

void MessageItemPrivate::setIsLoading(const bool isLoading)
{
    if (_isLoading != isLoading)
    {
        _isLoading = isLoading;
        emitPropertyChanged("isLoading");
    }
}

bool MessageItemPrivate::isSending() const
{
    return _id < 0;
}

bool MessageItemPrivate::deliveryReport() const
{
    return _deliveryReport;
}

void MessageItemPrivate::setDeliveryReport(const bool deliveryReport)
{
    if (_deliveryReport != deliveryReport)
    {
        _deliveryReport = deliveryReport;
        emitPropertyChanged("deliveryReport");
    }
}

QString MessageItemPrivate::title() const
{
    return _title;
}

void MessageItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

QString MessageItemPrivate::body() const
{
    return _body;
}

QString MessageItemPrivate::plainBody() const
{
    QString res = Emoticons::instance()->toEmoji(_body);
    res.replace("<br>", "\n");
    return Utils::fromHtmlEscaped(res);
}

QString MessageItemPrivate::shortBody() const
{
    if (!_body.isEmpty())
    {
        if (!_emoji)
        {
            return _body.left(60).replace("<br>", " ");
        }
        else
        {
            QString text = _body;
            return text.replace("<br>", " ");
        }
    }
    else if (_attachments && _attachments->count())
    {
        return _attachments->description();
    }
    else
    {
        return "...";
    }
}

void MessageItemPrivate::setBody(const QString &body, const bool emoji, const bool escaped, const bool simplified)
{
    if (escaped)
    {
        _body = Utils::toHtmlEscaped(body);
    }
    else
    {
        _body = body;
    }

    _body.replace("\n", "<br>");

    if (emoji)
    {
        _body = Emoticons::instance()->fromEmoji(_body);
    }

    if (simplified)
    {
        _body = Utils::simplified(_body);
    }

    _emoji = emoji;

    emitPropertyChanged("body");
}

int MessageItemPrivate::chatId() const
{
    return _chatId;
}

void MessageItemPrivate::setChatId(const int chatId)
{
    if (_chatId != chatId)
    {
        _chatId = chatId;
        emitPropertyChanged("chatId");
    }
}

bool MessageItemPrivate::isGroupChat() const
{
    return _chatId == 0 ? false : true;
}

bool MessageItemPrivate::emoji() const
{
    return _emoji;
}

AttachmentList* MessageItemPrivate::attachments() const
{
    return _attachments;
}

void MessageItemPrivate::setAttachments(AttachmentList *attachments)
{
    if (_attachments != attachments)
    {
        if (_attachments) delete _attachments;

        _attachments = attachments;
        emitPropertyChanged("attachments");
    }
}

void MessageItemPrivate::getAllFields(Connection *connection)
{
    setIsLoading(true);

    Packet *packet = new Packet("messages.getById");
    packet->addParam("message_ids", _id);

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    connection->appendQuery(packet);
}

void MessageItemPrivate::loadFinished(const Packet *sender, const QVariantMap &result)
{
    setIsLoading(false);

    QVariantMap response = result.value("response").toMap();
    MessageParser::parser(response.value("items").toList().at(0).toMap(), this);

    delete sender;
}
