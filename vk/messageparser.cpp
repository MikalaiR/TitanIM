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

MessageItem *MessageParser::parser(QVariantMap json)
{
    MessageItem *message = new MessageItem();

    message->setMid(json.contains("mid") ? json["mid"].toInt() : -1);
    message->setUid(json["uid"].toInt());
    message->setDate(QDateTime::fromTime_t(json["date"].toUInt()).toLocalTime());
    message->setIsUnread((json.contains("read_state") && !json["read_state"].toInt()) ? true : false);
    message->setIsOut((json.contains("out") && json["out"].toInt() == 1) ? true : false);
    message->setBody(Utils::decode(json["body"].toString()));
    message->setTitle(json["title"].toString());
    message->setChatId(json.contains("chat_id") ? json["chat_id"].toInt() : -1);
    message->setChatUsersCount(json.contains("users_count") ? json["users_count"].toInt() : 0);
    message->setChatActive(json.contains("chat_active") ? json["chat_active"].toString() : "");

    return message;
}
