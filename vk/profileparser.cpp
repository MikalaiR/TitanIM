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

ProfileItem* ProfileParser::parser(QVariantMap json)
{
    ProfileItem *profile = new ProfileItem();

    profile->setUid(json["uid"].toInt());
    profile->setFirstName(json["first_name"].toString());
    profile->setLastName(json["last_name"].toString());
    profile->setSex(json.contains("sex") ? (Sex)(json["sex"].toInt()) : unknown);
    profile->setPhotoMediumRect(json.contains("photo_medium_rec") ? Utils::photoUrlFix(json["photo_medium_rec"].toString())
                                                                 : Utils::photoUrlFix(json["photo_medium"].toString()));
    profile->setLastSeen(json.contains("last_seen") ? json["last_seen"].toMap()["time"].toInt() : 0);
    profile->setOnline(json["online"].toInt() == 1 ? true : false);

    return profile;
}
