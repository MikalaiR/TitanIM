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

MessageItem* MessageParser::parser(const QVariantMap &item)
{
    MessageItem *message = new MessageItem();

    int mid = -1;
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
    QString title = item.value("title").toString();
    int chatId = item.contains("chat_id") ? item.value("chat_id").toInt() : -1;

    message->setMid(mid);
    message->setUid(uid);
    message->setDate(date);
    message->setIsUnread(isUnread);
    message->setIsOut(isOut);
    message->setBody(body);
    message->setTitle(title);
    message->setChatId(chatId);

    return message;
}

MessageItem* MessageParser::parser(const QVariantMap &item, const ProfileList *profiles)
{
    MessageItem *message = parser(item);

    if (profiles && profiles->count()){
        ProfileItem *profile = profiles->at(message->uid());

        QString firstName = profile->firstName();
        QString lastName = profile->lastName();
        QString displayName = message->isGroupChat() ? message->title() : profile->fullName();
        Sex sex = profile->sex();
        QString photoMediumRect = profile->photoMediumRect();
        int lastSeen = profile->lastSeen();

        bool online = false;
        if (message->lastSeen() != 0 && !message->isGroupChat())
        {
            bool online = profile->online();
        }

        message->setFirstName(firstName);
        message->setLastName(lastName);
        message->setDisplayName(displayName);
        message->setSex(sex);
        message->setPhotoMediumRect(photoMediumRect);
        message->setLastSeen(lastSeen);
        message->setOnline(online);
    }

    return message;
}

MessageList* MessageParser::parser(const QVariantList &items, const ProfileList *profiles)
{
    MessageList *messages = new MessageList();

    foreach (QVariant item, items)
    {
        MessageItem *message = parser(item.toMap(), profiles);
        messages->add(message);
    }

    return messages;
}
