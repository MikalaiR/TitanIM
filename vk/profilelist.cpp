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

#include "profilelist.h"

ProfileListPrivate::ProfileListPrivate()
{
}

ProfileListPrivate::~ProfileListPrivate()
{
}

void ProfileListPrivate::add(ProfileItem profile)
{
    _profiles.append(profile);
    connect(profile.data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
}

void ProfileListPrivate::add(const QVector<ProfileItem> &profiles)
{
    for (int i = 0; i < profiles.count(); i++)
    {
        add(profiles.at(i));
    }
}

int ProfileListPrivate::indexOf(const int uid) const
{
    for (int i = 0; i < _profiles.count(); i++)
    {
        if (_profiles.at(i)->uid() == uid)
            return i;
    }

    return -1;
}

ProfileItem ProfileListPrivate::at(const int i) const
{
    return _profiles.at(i);
}

ProfileItem ProfileListPrivate::item(const int uid) const
{
    foreach (ProfileItem item, _profiles)
    {
        if (item->uid() == uid)
            return item;
    }

    return ProfileItem::create();
}

void ProfileListPrivate::remove(const int i)
{
    disconnect(_profiles.at(i).data(), SIGNAL(propertyChanged(int,QString)), this, SLOT(onItemChanged(int,QString)));
    _profiles.remove(i);
}

int ProfileListPrivate::count() const
{
    return _profiles.count();
}

QVector<ProfileItem> ProfileListPrivate::toVector() const
{
    return _profiles;
}

void ProfileListPrivate::onItemChanged(const int uid, const QString &propertyName)
{
    emit itemChanged(indexOf(uid));
}
