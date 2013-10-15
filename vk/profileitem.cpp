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

ProfileItem::ProfileItem()
{
}

int ProfileItem::uid()
{
    return _uid;
}

void ProfileItem::setUid(const int uid)
{
    if (_uid != uid)
    {
        _uid = uid;
    }
}

QString ProfileItem::firstName()
{
    return _firstName;
}

void ProfileItem::setFirstName(const QString &firstName)
{
    if (_firstName != firstName)
    {
        _firstName = firstName;
    }
}

QString ProfileItem::lastName()
{
    return _lastName;
}

void ProfileItem::setLastName(const QString &lastName)
{
    if (_lastName != lastName)
    {
        _lastName = lastName;
    }
}

QString ProfileItem::fullName()
{
    return _firstName + " " + _lastName;
}

Sex ProfileItem::sex()
{
    return _sex;
}

void ProfileItem::setSex(const Sex sex)
{
    if (_sex != sex)
    {
        _sex = sex;
    }
}

QString ProfileItem::photoMediumRect()
{
    return _photoMediumRect;
}

void ProfileItem::setPhotoMediumRect(const QString &photoMediumRect)
{
    if (_photoMediumRect != photoMediumRect)
    {
        _photoMediumRect = photoMediumRect;
    }
}

bool ProfileItem::online()
{
    return _online;
}

void ProfileItem::setOnline(const bool online)
{
    if (_online != online)
    {
        _online = online;
    }
}

int ProfileItem::lastSeen()
{
    return _lastSeen;
}

void ProfileItem::setLastSeen(const int lastSeen)
{
    if (_lastSeen != lastSeen)
    {
        _lastSeen = lastSeen;
    }
}
