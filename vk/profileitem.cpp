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
    _id = -1;
    _sex = unknown;
    _online = false;
    _lastSeen = 0;
}

int ProfileItemPrivate::id() const
{
    return _id;
}

void ProfileItemPrivate::setId(const int id)
{
    if (_id != id)
    {
        _id = id;
        emit propertyChanged(_id, "id");
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
        emit propertyChanged(_id, "firstName");
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
        emit propertyChanged(_id, "lastName");
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
        emit propertyChanged(_id, "sex");
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
        emit propertyChanged(_id, "photoMediumRect");
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
        emit propertyChanged(_id, "online");
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
        emit propertyChanged(_id, "lastSeen");
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
        emit propertyChanged(_id, "activity");
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
