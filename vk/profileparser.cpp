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
    int sex = item.contains("sex") ? item.value("sex").toInt() : 0;
    QString photoMediumRect = item.contains("photo_100") ? item.value("photo_100").toString()
                                                         : item.value("photo_medium_rec").toString();
    int lastSeen = item.contains("last_seen") ? item.value("last_seen").toMap().value("time").toInt() : 0;
    bool online = item.contains("online") ? item.value("online").toInt() : 0;
    QString activity = item.contains("status") ? item.value("status").toString() : "";
    QString bdate = item.contains("bdate") ? item.value("bdate").toString() : "";
    QString domain = item.value("domain").toString();
    QString city = item.contains("city") ? item.value("city").toMap().value("title").toString() : "";
    QString mobilePhone = item.contains("mobile_phone") ? item.value("mobile_phone").toString() : "";
    QString alphabet = !firstName.isEmpty() ? QString(firstName.at(0)) : "";
    int friendStatus = item.contains("friend_status") ? item.value("friend_status").toInt() : 0;
    bool blacklistedByMe = item.contains("blacklisted_by_me") ? item.value("blacklisted_by_me").toInt() : 0;

    profile->beginChangeGroupProperties();

    profile->setId(uid);
    profile->setFullName(firstName, lastName);
    profile->setSex((ProfileItemPrivate::Sex)sex);
    profile->setPhotoMediumRect(photoMediumRect);
    profile->setLastSeen(lastSeen);
    profile->setOnline(online);
    profile->setActivity(activity);
    profile->setBdate(bdate);
    profile->setDomain(domain);
    profile->setCity(city);
    profile->setMobilePhone(mobilePhone);
    profile->setAlphabet(alphabet);
    profile->setBlacklistedByMe(blacklistedByMe);
    profile->setFriendStatus((ProfileItemPrivate::FriendStatus)friendStatus);

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

    for (QVariant item: items)
    {
        profiles->add(parser(item.toMap(), isCached));
    }

    return profiles;
}
