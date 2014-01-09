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
}

void HistoryPacket::load(const int id, const int offset, const int count)
{
    Packet *packet = new Packet("messages.getHistory");
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    packet->addParam("user_id", id);
    packet->setId(id);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

void HistoryPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    MessageList messageList = MessageParser::parser(response.value("items").toList());

    emit history(this, sender->id(), messageList);
    delete sender;
}
