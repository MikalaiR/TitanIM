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

#include "profileitem.h"
#include "profileparser.h"

ProfileItemPrivate::ProfileItemPrivate()
{
    _sex = Unknown;
    _online = false;
    _lastSeen = 0;
    _isLoading = false;

    connect(this, SIGNAL(propertyChanged(int,QString)), this, SIGNAL(fullNameChanged())); //todo
    connect(this, SIGNAL(propertyChanged(int,QString)), this, SIGNAL(photoMediumRectChanged())); //todo
    connect(this, SIGNAL(propertyChanged(int,QString)), this, SIGNAL(sexChanged())); //todo
}

QString ProfileItemPrivate::firstName() const
{
    return _firstName;
}

void ProfileItemPrivate::setFirstName(const QString &firstName)
{
    if (_firstName != firstName)
    {
        _firstName = firstName;
        emitPropertyChanged("firstName");
    }
}

QString ProfileItemPrivate::lastName() const
{
    return _lastName;
}

void ProfileItemPrivate::setLastName(const QString &lastName)
{
    if (_lastName != lastName)
    {
        _lastName = lastName;
        emitPropertyChanged("lastName");
    }
}

QString ProfileItemPrivate::fullName() const
{
    return _firstName + " " + _lastName;
}

ProfileItemPrivate::Sex ProfileItemPrivate::sex() const
{
    return _sex;
}

void ProfileItemPrivate::setSex(const Sex sex)
{
    if (_sex != sex)
    {
        _sex = sex;
        emitPropertyChanged("sex");
    }
}

QString ProfileItemPrivate::photoMediumRect() const
{
    return _photoMediumRect;
}

void ProfileItemPrivate::setPhotoMediumRect(const QString &photoMediumRect)
{
    if (_photoMediumRect != photoMediumRect)
    {
        _photoMediumRect = photoMediumRect;
        emitPropertyChanged("photoMediumRect");
    }
}

bool ProfileItemPrivate::online() const
{
    return _online;
}

void ProfileItemPrivate::setOnline(const bool online)
{
    if (_online != online)
    {
        _online = online;
        emitPropertyChanged("online");
    }
}

int ProfileItemPrivate::lastSeen() const
{
    return _lastSeen;
}

void ProfileItemPrivate::setLastSeen(const int lastSeen)
{
    if (_lastSeen != lastSeen)
    {
        _lastSeen = lastSeen;
        emitPropertyChanged("lastSeen");
    }
}

QString ProfileItemPrivate::lastSeenText() const
{
    return _online ? tr("online") : Utils::lastSeenToString(_lastSeen, _sex);
}

QString ProfileItemPrivate::activity() const
{
    return _activity;
}

void ProfileItemPrivate::setActivity(const QString &activity)
{
    if (_activity != activity)
    {
        _activity = activity;
        emitPropertyChanged("activity");
    }
}

QString ProfileItemPrivate::alphabet() const
{
    return _alphabet;
}

void ProfileItemPrivate::setAlphabet(const QString &alphabet)
{
    _alphabet = alphabet;
}

bool ProfileItemPrivate::isLoading() const
{
    return _isLoading;
}

void ProfileItemPrivate::join(const ProfileItem other)
{
    if (other && !other->firstName().isEmpty())
    {
        setFirstName(other->firstName());
        setLastName(other->lastName());
        setSex(other->sex());
        setPhotoMediumRect(other->photoMediumRect());
        setOnline(other->online());
        setLastSeen(other->lastSeen());
    }
}

void ProfileItemPrivate::setIsLoading(const bool isLoading)
{
    if (_isLoading != isLoading)
    {
        _isLoading = isLoading;
        emitPropertyChanged("isLoading");
    }
}

void ProfileItemPrivate::getAllFields(Connection *connection)
{
    setIsLoading(true);

    Packet *packet = new Packet("users.get");
    packet->addParam("user_ids", _id);
    packet->addParam("fields", "photo_100,online,last_seen,sex");

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    connection->appendQuery(packet);
}

void ProfileItemPrivate::updateLastSeenText()
{
    if (!_online)
    {
        emitPropertyChanged("lastSeen");
    }
}

void ProfileItemPrivate::loadFinished(const Packet *sender, const QVariantMap &result)
{
    setIsLoading(false);

    QVariantList response = result.value("response").toList();

    if (response.count())
    {
        ProfileParser::parser(response.at(0).toMap(), this);
    }
    else
    {
        setFirstName("DELETED");
        setPhotoMediumRect("http://vk.com/images/camera_b.gif");
    }

    delete sender;
}
