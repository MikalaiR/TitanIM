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

#include "audioparser.h"

AudioItem AudioParser::parser(const QVariantMap &item)
{
    AudioItem audio = AudioItem::create();

    int id = item.value("id").toInt();
    int ownerId = item.value("owner_id").toInt();
    QString artist = item.value("artist").toString();
    QString title = item.value("title").toString();
    int duration = item.value("duration").toInt();
    QUrl url = item.value("url").toUrl();

    audio->setId(id);
    audio->setOwnerId(ownerId);
    audio->setArtist(artist);
    audio->setTitle(title);
    audio->setDuration(duration);
    audio->setUrl(url);

    return audio;
}

AudioList AudioParser::parser(const QVariantList &items)
{
    AudioList audioList = AudioList::create();

    foreach (QVariant item, items)
    {
        AudioItem audio = parser(item.toMap());
        audioList->add(audio);
    }

    return audioList;
}
