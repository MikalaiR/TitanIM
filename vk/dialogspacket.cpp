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

#include "dialogspacket.h"

DialogsPacket::DialogsPacket(Connection *connection)
{
    _connection = connection;
}

void DialogsPacket::load(const int offset, const int count)
{
    Packet *packet = new Packet("execute");
    QString script;
    _offset = offset;
    _count = count;

    if (offset == 0)
    {
        script = "var d=API.messages.getDialogs({\"offset\":" + QString::number(offset) + ",\"count\":" + QString::number(count) + ",\"preview_length\":50});"
               + "var p=API.users.get({\"user_ids\":d.items@.user_id+d.items@.chat_active,\"fields\":\"photo_medium_rec,online,last_seen,sex\"});"
               + "var m=API.messages.get({\"filters\":1,\"preview_length\":1});"
               + "var f=API.friends.getRequests({\"count\":1000});"
               + "return {\"dialogs\":d,\"profiles\":p,\"countNewMsg\":m.count,\"countNewFriends\":f.count};";
    }
    else
    {
        script = "var d=API.messages.getDialogs({\"offset\":" + QString::number(offset) + ",\"count\":" + QString::number(count) + ",\"preview_length\":50});"
               + "var p=API.getProfiles({\"uids\":d@.uid+d@.chat_active,\"fields\":\"photo_medium_rec,online,last_seen,sex\"});"
               + "return {\"dialogs\":d,\"profiles\":p};";
    }

    packet->addParam("code", script);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

int DialogsPacket::offset() const
{
    return _offset;
}

int DialogsPacket::count() const
{
    return _count;
}

void DialogsPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    ProfileList profiles = ProfileParser::parser(response.value("profiles").toList());
    MessageList messageList = MessageParser::parser(response.value("dialogs").toMap()["items"].toList(), profiles);

    int countNewMessages = response.value("countNewMsg").toInt();
    int countRequestsFriends = response.value("countNewFriends").toInt();

    emit dialogs(this, messageList);
    delete sender;
}
