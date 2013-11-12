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

ProfileItem* ProfileParser::parser(const QVariantMap &item)
{
    ProfileItem *profile = new ProfileItem();

    int uid = item.contains("id") ? item.value("id").toInt() : item.value("uid").toInt();
    QString firstName = item.value("first_name").toString();
    QString lastName = item.value("last_name").toString();
    Sex sex = item.contains("sex") ? (Sex)(item.value("sex").toInt()) : unknown;
    QString photoMediumRect = item.contains("photo_medium_rec") ? item.value("photo_medium_rec").toString()
                                                                : item.value("photo_medium").toString();
    int lastSeen = item.contains("last_seen") ? item.value("last_seen").toMap().value("time").toInt() : 0;
    bool online = item.value("online").toInt() == 1 ? true : false;

    profile->setUid(uid);
    profile->setFirstName(firstName);
    profile->setLastName(lastName);
    profile->setSex(sex);
    profile->setPhotoMediumRect(photoMediumRect);
    profile->setLastSeen(lastSeen);
    profile->setOnline(online);

    return profile;
}

ProfileList* ProfileParser::parser(const QVariantList &items)
{
    ProfileList *profiles = new ProfileList();

    foreach (QVariant item, items)
    {
        ProfileItem *profile = parser(item.toMap());
        profiles->add(profile);
    }

    return profiles;
}
