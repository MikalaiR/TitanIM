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
    _fields = "photo_100,online,last_seen,sex,domain,bdate,city,contacts,friend_status,blacklisted_by_me";
}

void DialogsPacket::load(const int offset, const int count)
{
    _offset = offset;
    _count = count;

    Packet *packet = new Packet("execute.messagesGetDialogs");
    packet->addParam("offset", _offset);
    packet->addParam("count", _count);
    packet->addParam("preview_length", 50);
    packet->addParam("fields", _fields);

    connect(packet, &Packet::finished, this, &DialogsPacket::loadFinished);
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
