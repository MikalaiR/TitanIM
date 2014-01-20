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

MessageItem MessageParser::parser(const QVariantMap &item)
{
    MessageItem message = MessageItem::create();

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
    QString body = Utils::decode(item.value("body").toString());
    QString title = item.contains("title") ? item.value("title").toString() : "";
    int chatId = item.contains("chat_id") ? item.value("chat_id").toInt() : 0;

    if (item.contains("attachments"))
    {
        AttachmentList attachments = AttachmentsParser::parser(item.value("attachments").toList());
        message->setAttachments(attachments);
    }

    message->setId(mid);
    message->setUid(uid);
    message->setDate(date);
    message->setIsUnread(isUnread);
    message->setIsOut(isOut);
    message->setBody(body);
    message->setTitle(title);
    message->setChatId(chatId);

    return message;
}

MessageList MessageParser::parser(const QVariantList &items)
{
    MessageList messages = MessageList::create();

    foreach (QVariant item, items)
    {
        MessageItem message = parser(item.toMap());
        messages->add(message);
    }

    return messages;
}
