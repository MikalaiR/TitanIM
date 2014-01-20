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

#include "photoparser.h"

PhotoItem PhotoParser::parser(const QVariantMap &item)
{
    PhotoItem photo = PhotoItem::create();

    int pid = item.contains("id") ? item.value("id").toInt() : item.value("pid").toInt();
    int aid = item.contains("album_id") ? item.value("album_id").toInt() : item.value("aid").toInt();
    int ownerId = item.value("owner_id").toInt();

    QVariantMap photoItem;
    QString src;
    QString srcBig;

    if (item.contains("sizes"))
    {
        src = item.value("sizes").toList().at(1).toMap().value("src").toString();
        photoItem = item.value("sizes").toList().at(2).toMap();
        srcBig = photoItem.value("src").toString();
    }
    else
    {
        src = item.value("photo_130").toString();
        srcBig = item.value("photo_604").toString();
        photoItem = item;
    }

    int width = 0;
    int height = 0;

    if (photoItem.contains("width"))
    {
        width = photoItem.value("width").toInt();
        height = photoItem.value("height").toInt();
    }

    photo->setId(pid);
    photo->setAlbumId(aid);
    photo->setOwnerId(ownerId);
    photo->setSrc(src);
    photo->setSrcBig(srcBig);
    photo->setWidth(width);
    photo->setHeight(height);

    return photo;
}
