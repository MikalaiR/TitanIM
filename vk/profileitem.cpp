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

ProfileItemPrivate::ProfileItemPrivate()
{
}

int ProfileItemPrivate::uid() const
{
    return _uid;
}

void ProfileItemPrivate::setUid(const int uid)
{
    if (_uid != uid)
    {
        _uid = uid;
    }
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
    }
}

QString ProfileItemPrivate::fullName() const
{
    return _firstName + " " + _lastName;
}

Sex ProfileItemPrivate::sex() const
{
    return _sex;
}

void ProfileItemPrivate::setSex(const Sex sex)
{
    if (_sex != sex)
    {
        _sex = sex;
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
    }
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