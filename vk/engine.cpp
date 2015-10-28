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
    connect(_connection, SIGNAL(authorized(int,QString,QString)), this, SLOT(onAuthorized(int,QString,QString)));//todo
    connect(_connection, SIGNAL(logout(int)), this, SLOT(onLogout(int)));

    _profiles = new QHash<int, ProfileItem>();
    _photosPacket = 0;
    _maxMsgId = 0;

    _selfProfile = ProfileItem::create();
    QQmlEngine::setObjectOwnership(_selfProfile.data(), QQmlEngine::CppOwnership);
}

Engine::~Engine()
{
    delete _profiles;
    if (_photosPacket) delete _photosPacket;
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
    packet->setPerishable(true);
    packet->addParam("order", "hints");
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onFriendsOnline(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

void Engine::getSelfProfile()
{
    _selfProfile->getAllFields(_connection);
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

void Engine::updateUser(const int id)
{
    if (_profiles->contains(id))
    {
        _profiles->value(id)->getAllFields(_connection);
    }
}

void Engine::getPhotosProfile(const int uid)
{
    if (!_photosPacket)
    {
        _photosPacket = new PhotosPacket(_connection);
        connect(_photosPacket, SIGNAL(photos(int,QList<PhotoItem>)), this, SLOT(onGetPhotosProfile(int,QList<PhotoItem>)));
    }

    _photosPacket->getPhotos(uid, "profile");
}

QVariantList Engine::photosProfile(const int uid)
{
    //todo uid. not only current profile

    QVariantList list;

    foreach (PhotoItem photo, _currentPhotosProfile) {
        list.append(QVariant::fromValue(photo.data()));
    }

    return list;
}

int Engine::maxMsgId() const
{
    return _maxMsgId;
}

void Engine::setMaxMsgId(const int mid)
{
    if (mid > _maxMsgId)
    {
        _maxMsgId = mid;
    }
}

void Engine::banUser(const int uid)
{
    Packet *packet = new Packet("account.banUser");
    packet->setId(uid);
    packet->addParam("user_id", uid);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onBanUser(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);

    if (_profiles->contains(uid))
    {
        _profiles->value(uid)->getAllFields(_connection);
    }
}

void Engine::unbanUser(const int uid)
{
    Packet *packet = new Packet("account.unbanUser");
    packet->setId(uid);
    packet->addParam("user_id", uid);
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onUnbanUser(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);

    if (_profiles->contains(uid))
    {
        _profiles->value(uid)->getAllFields(_connection);
    }
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

void Engine::onGetPhotosProfile(const int id, const QList<PhotoItem> &photos)
{
    _currentPhotosProfile = photos;

    emit photosProfileLoaded(id);
}

void Engine::onBanUser(const Packet *sender, const QVariantMap &result)
{
    int id = sender->id();

    if (result.value("response").toInt() == 1 && _profiles->contains(id))
    {
        _profiles->value(id)->setBlacklistedByMe(true);
    }

    delete sender;
}

void Engine::onUnbanUser(const Packet *sender, const QVariantMap &result)
{
    int id = sender->id();

    if (result.value("response").toInt() == 1 && _profiles->contains(id))
    {
        _profiles->value(id)->setBlacklistedByMe(false);
    }

    delete sender;
}

void Engine::onAuthorized(const int uid, const QString &token, const QString &secret)
{
    _selfProfile->setId(uid);
    getSelfProfile();
    _profiles->insert(uid, _selfProfile);
}

void Engine::onLogout(const int uid)
{
    _profiles->clear();
    _maxMsgId = 0;
}
