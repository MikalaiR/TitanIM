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

#include "audioitem.h"

AudioItemPrivate::AudioItemPrivate()
{
    setAttachmentType(Audio);

    _duration = 0;
}

QString AudioItemPrivate::artist() const
{
    return _artist;
}

void AudioItemPrivate::setArtist(const QString &artist)
{
    if (_artist != artist)
    {
        _artist = artist;
        emitPropertyChanged("artist");
    }
}

QString AudioItemPrivate::title() const
{
    return _title;
}

void AudioItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

int AudioItemPrivate::duration() const
{
    return _duration;
}

QString AudioItemPrivate::durationStr() const
{
    if (!_duration)
    {
        return "";
    }

    return Utils::durationToText(_duration);
}

void AudioItemPrivate::setDuration(const int duration)
{
    if (_duration != duration)
    {
        _duration = duration;
        emitPropertyChanged("duration");
    }
}

QUrl AudioItemPrivate::url() const
{
    return _url;
}

void AudioItemPrivate::setUrl(const QUrl &url)
{
    if (_url != url)
    {
        _url = url;
        emitPropertyChanged("url");
    }
}
