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

#include "profileparser.h"

void ProfileParser::parser(const QVariantMap &item, ProfileItemPrivate *profile)
{
    int uid = item.contains("id") ? item.value("id").toInt() : item.value("uid").toInt();
    QString firstName = item.value("first_name").toString();
    QString lastName = item.value("last_name").toString();
    Sex sex = item.contains("sex") ? (Sex)(item.value("sex").toInt()) : Unknown;
    QString photoMediumRect = item.contains("photo_100") ? item.value("photo_100").toString()
                                                         : item.value("photo_medium_rec").toString();
    int lastSeen = item.contains("last_seen") ? item.value("last_seen").toMap().value("time").toInt() : 0;
    bool online = item.value("online").toInt() == 1 ? true : false;
    QString activity = item.contains("status") ? item.value("status").toString() : "";
    QString alphabet = !firstName.isEmpty() ? QString(firstName.at(0)) : "";

    profile->beginChangeGroupProperties();

    profile->setId(uid);
    profile->setFirstName(firstName);
    profile->setLastName(lastName);
    profile->setSex(sex);
    profile->setPhotoMediumRect(photoMediumRect);
    profile->setLastSeen(lastSeen);
    profile->setOnline(online);
    profile->setActivity(activity);
    profile->setAlphabet(alphabet);

    profile->endChangeGroupProperties();
}

void ProfileParser::parser(const QVariantMap &item, ProfileItem profile)
{
    parser(item, profile.data());
}

ProfileItem ProfileParser::parser(const QVariantMap &item, const bool isCached)
{
    ProfileItem profile = ProfileItem::create();
    parser(item, profile.data());

    if (isCached)
    {
        return Client::instance()->engine()->getProfile(profile->id(), profile);
    }
    else
    {
        return profile;
    }
}

ProfileList ProfileParser::parser(const QVariantList &items, const bool isCached)
{
    ProfileList profiles = ProfileList::create();

    foreach (QVariant item, items)
    {
        profiles->add(parser(item.toMap(), isCached));
    }

    return profiles;
}
