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

    _offset = 0;
    _count = 0;
    _serverCount = 0;
    _unreadDialogs = 0;
    _fields = "photo_100,online,last_seen,sex";
}

void DialogsPacket::load(const int offset, const int count)
{
    Packet *packet = new Packet("execute");
    _offset = offset;
    _count = count;

    QString script = "var d=API.messages.getDialogs({\"offset\":" + QString::number(offset)
                   + ",\"count\":" + QString::number(count) + ",\"preview_length\":50});"
                   + "var i=0;var s=\"\";"
                   + "while(i<d.items.length){s=s+\",\"+d.items[i].message.chat_active;i=i+1;};"
                   + "var p=API.users.get({\"user_ids\":d.items@.message@.user_id+s,\"fields\":\"" + _fields + "\"});"
                   + "return {\"dialogs\":d,\"profiles\":p};";

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

int DialogsPacket::serverCount() const
{
    return _serverCount;
}

int DialogsPacket::unreadDialogs() const
{
    return _unreadDialogs;
}

QString DialogsPacket::fields() const
{
    return _fields;
}

void DialogsPacket::setFields(const QString &fields)
{
    _fields = fields;
}

void DialogsPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantMap dialogsItem = response.value("dialogs").toMap();

    ProfileList profiles = ProfileParser::parser(response.value("profiles").toList());
    DialogList dialogList = DialogParser::parser(dialogsItem.value("items").toList(), profiles);

    _serverCount = dialogsItem.value("count").toInt();
    _unreadDialogs = dialogsItem.contains("unread_dialogs") ? dialogsItem.value("unread_dialogs").toInt() : 0;

    emit dialogs(this, dialogList);
    delete sender;
}
