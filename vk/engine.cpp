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

#include "engine.h"

Engine::Engine(Connection *connection)
{
    _connection = connection;
    connect(_connection, SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(_connection, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    _profiles = new QHash<int, ProfileItem>();

    _selfProfile = ProfileItem::create();
    QQmlEngine::setObjectOwnership(_selfProfile.data(), QQmlEngine::CppOwnership);
}

Engine::~Engine()
{
    delete _profiles;
}

int Engine::uid() const
{
    return _selfProfile->id();
}

void Engine::setUserOnline(const int uid)
{
    if (_profiles->contains(uid))
    {
        _profiles->value(uid)->setOnline(true);
    }
}

void Engine::setUserOffline(const int uid, const bool isAway)
{
    if (_profiles->contains(uid))
    {
        _profiles->value(uid)->setOnline(false);

        int min = 0;

        if (isAway)
        {
            min -= USER_OFFLINE_AWAY;
        }

        _profiles->value(uid)->setLastSeen(Utils::currentDateTime().addSecs(min * 60).toTime_t());
    }
}

void Engine::getFriendsOnline()
{
    Packet *packet = new Packet("friends.getOnline");
    packet->addParam("order", "hints");
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onFriendsOnline(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

ProfileItem Engine::getProfile() const
{
    return _selfProfile;
}

QVariant Engine::getUser()
{
    return QVariant::fromValue(_selfProfile.data());
}

ProfileItem Engine::getProfile(const int id, ProfileItem defaultValue)
{
    if (_profiles->contains(id))
    {
        ProfileItem profile = _profiles->value(id);
        profile->join(defaultValue);
        return profile;
    }
    else
    {
        if (!defaultValue)
        {
            defaultValue = ProfileItem::create();
            defaultValue->setId(id);
            defaultValue->getAllFields(_connection);
        }

        _profiles->insert(id, defaultValue);
        return defaultValue;
    }
}

QVariant Engine::getUser(const int id)
{
    ProfileItem profile = getProfile(id);
    QQmlEngine::setObjectOwnership(profile.data(), QQmlEngine::CppOwnership);

    return QVariant::fromValue(profile.data());
}

void Engine::onFriendsOnline(const Packet *sender, const QVariantMap &result)
{
    QVariantList response = result.value("response").toList();
    QSet<int> uids;

    foreach (QVariant item, response)
    {
        uids.insert(item.toInt());
    }

    QHash<int, ProfileItem>::const_iterator i = _profiles->constBegin();
    while (i != _profiles->constEnd())
    {
        ProfileItem profile = i.value();
        profile->setOnline(uids.contains(profile->id()));

        if (!profile->online())
        {
            profile->setLastSeen(0);
        }

        ++i;
    }

    delete sender;
}

void Engine::onConnected(const int uid, const QString &token, const QString &secret)
{
    _selfProfile->setId(uid);
    _selfProfile->getAllFields(_connection);
    _profiles->insert(uid, _selfProfile);
}

void Engine::onDisconnected()
{
}
