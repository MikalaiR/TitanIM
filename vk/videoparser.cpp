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

#include "videoparser.h"

VideoItem VideoParser::parser(const QVariantMap &item)
{
    VideoItem video = VideoItem::create();

    int id = item.value("id").toInt();
    int ownerId = item.value("owner_id").toInt();
    QString title = item.value("title").toString();
    QString description = item.value("description").toString();
    int duration = item.value("duration").toInt();
    QUrl src = item.value("photo_130").toUrl();
    QUrl srcBig = item.value("photo_320").toUrl();
    QString accessKey = item.value("access_key").toString();

    video->setId(id);
    video->setOwnerId(ownerId);
    video->setTitle(title);
    video->setDescription(description);
    video->setDuration(duration);
    video->setSrc(src);
    video->setSrcBig(srcBig);
    video->setAccessKey(accessKey);

    return video;
}
