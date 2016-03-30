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

#include "rosterpacket.h"

RosterPacket::RosterPacket(Connection *connection)
{
    _connection = connection;
    _fields = "photo_100,online,last_seen,sex,domain,bdate,city,contacts";
    _needFavorites = false;
}

void RosterPacket::load(const int offset, const int count)
{
    Packet *packet;
    _offset = offset;
    _count = count;

    if (_offset == 0 && _needFavorites)
    {
        packet = new Packet("execute.friendsGet");
    }
    else
    {
        packet = new Packet("friends.get");
    }

    packet->addParam("fields", _fields);
    packet->addParam("offset", _offset);
    packet->addParam("count", _count);
    packet->addParam("order", "name");

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

int RosterPacket::offset() const
{
    return _offset;
}

int RosterPacket::count() const
{
    return _count;
}

QString RosterPacket::fields() const
{
    return _fields;
}

void RosterPacket::setFields(const QString &fields)
{
    _fields = fields;
}

bool RosterPacket::needFavorites() const
{
    return _needFavorites;
}

void RosterPacket::setNeedFavorites(const bool needFavorites)
{
    _needFavorites = needFavorites;
}

void RosterPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    ProfileList profiles;

    int k = 0;

    if (sender->method() == "execute.friendsGet")
    {
        profiles = ProfileList::create();

        foreach (QVariant item, response.value("favorites").toMap().value("items").toList())
        {
            ProfileItem profile = ProfileParser::parser(item.toMap());
            profile->setFriendStatus(ProfileItemPrivate::Friend);
            profile->setTop(true);
            profiles->add(profile);
        }

        k = profiles->count();
        profiles->add(ProfileParser::parser(response.value("friends").toMap().value("items").toList())->toList());
    }
    else
    {
        profiles = ProfileParser::parser(response.value("items").toList());
    }

    for (int i = k; i < profiles->count(); i++)
    {
        profiles->at(i)->setFriendStatus(ProfileItemPrivate::Friend);

        //remove dublicate top profile
        if (profiles->at(i)->isTop())
        {
            profiles->removeAt(i);
            i--;
        }
    }

    emit roster(this, profiles);
    delete sender;
}
