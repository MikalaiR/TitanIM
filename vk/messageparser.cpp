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

#include "messageparser.h"

void MessageParser::parser(const QVariantMap &item, MessageItemPrivate *message)
{
    int mid = 0;
    if (item.contains("id"))
    {
        mid = item.value("id").toInt();
    }
    else if (item.contains("mid"))
    {
        mid = item.value("mid").toInt();
    }

    int uid = item.contains("user_id") ? item.value("user_id").toInt() : item.value("uid").toInt();
    QDateTime date = QDateTime::fromTime_t(item.value("date").toUInt()).toLocalTime();
    bool isUnread = (item.contains("read_state") && !item.value("read_state").toInt()) ? true : false;
    bool isOut = (item.contains("out") && item.value("out").toInt()) ? true : false;
    QString body = item.value("body").toString();
    QString title = item.contains("title") ? item.value("title").toString() : "";
    int chatId = item.contains("chat_id") ? item.value("chat_id").toInt() : 0;
    bool emoji = (item.contains("emoji") && item.value("emoji").toInt()) ? true : false;

    AttachmentList *attachments = 0;

    if (item.contains("attachments"))
    {
        attachments = AttachmentsParser::parser(item.value("attachments").toList());
    }

    if (item.contains("geo"))
    {
        if (!attachments)
        {
            attachments = new AttachmentList();
        }

        attachments->add(MapParser::parser(item.value("geo").toMap()));
    }

    message->beginChangeGroupProperties();

    message->setId(mid);
    message->setUid(uid);
    message->setDate(date);
    message->setIsUnread(isUnread);
    message->setDeliveryReport(true);
    message->setIsOut(isOut);
    message->setBody(body, emoji);
    message->setTitle(title);
    message->setChatId(chatId);
    message->setAttachments(attachments);

    message->endChangeGroupProperties();
}

void MessageParser::parser(const QVariantMap &item, MessageItem message)
{
    parser(item, message.data());
}

MessageItem MessageParser::parser(const QVariantMap &item)
{
    MessageItem message = MessageItem::create();
    parser(item, message.data());

    return message;
}

MessageList MessageParser::parser(const QVariantList &items)
{
    MessageList messages = MessageList::create();

    foreach (QVariant item, items)
    {
        MessageItem message = MessageItem::create();
        parser(item.toMap(), message.data());
        messages->add(message);
    }

    return messages;
}
