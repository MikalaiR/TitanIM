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

#include "historypacket.h"

HistoryPacket::HistoryPacket(Connection *connection)
{
    _connection = connection;

    _offset = 0;
    _count = 0;
    _serverCount = 0;
    _unreadCount = 0;
}

void HistoryPacket::load(const int id, const int offset, const int count)
{
    _offset = offset;
    _count = count;

    Packet *packet = new Packet("execute.messagesGetHistory");
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    packet->addParam("user_id", id);
    packet->addParam("fields", "photo_100,online,last_seen,sex,domain,bdate,city,contacts,friend_status,blacklisted_by_me");
    packet->setId(id);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

int HistoryPacket::offset() const
{
    return _offset;
}

int HistoryPacket::count() const
{
    return _count;
}

int HistoryPacket::serverCount() const
{
    return _serverCount;
}

int HistoryPacket::unreadCount() const
{
    return _unreadCount;
}

void HistoryPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantMap historyItem = response.value("history").toMap();

    MessageList messageList = MessageParser::parser(historyItem.value("items").toList());
    ProfileParser::parser(response.value("profiles").toList()); //add cache

    ProfileList profiles_act = ProfileParser::parser(response.value("profiles_act").toList(), false);
    if (profiles_act->count() > 0)
    {
        for (int i = 0; i < messageList->count(); i++)
        {
            MessageItem message = qobject_cast<MessageItem>(messageList->at(i));
            if (message->actionMid() > 0)
            {
                message->setActionText(profiles_act->item(message->actionMid())->fullName());
            }
        }
    }

    _serverCount = historyItem.value("count").toInt();
    _unreadCount = historyItem.contains("unread") ? historyItem.value("unread").toInt() : 0;

    emit history(this, sender->id(), messageList);
    delete sender;
}
