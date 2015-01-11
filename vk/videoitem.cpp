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

#include "videoitem.h"

VideoItemPrivate::VideoItemPrivate()
{
    setAttachmentType(Video);

    _duration = 0;
}

QString VideoItemPrivate::title() const
{
    return _title;
}

void VideoItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

QString VideoItemPrivate::description() const
{
    return _description;
}

void VideoItemPrivate::setDescription(const QString &description)
{
    if (_description != description)
    {
        _description = description;
        emitPropertyChanged("description");
    }
}

int VideoItemPrivate::duration() const
{
    return _duration;
}

QString VideoItemPrivate::durationStr() const
{
    int m = _duration / 60;
    int s = _duration - (m * 60);
    return QString("%1:%2").arg(m).arg(QString().number(s).rightJustified(2, '0'));
}

void VideoItemPrivate::setDuration(const int duration)
{
    if (_duration != duration)
    {
        _duration = duration;
        emitPropertyChanged("duration");
    }
}

QUrl VideoItemPrivate::src() const
{
    return _src;
}

void VideoItemPrivate::setSrc(const QUrl &src)
{
    if (_src != src)
    {
        _src = src;
        emitPropertyChanged("src");
    }
}

QUrl VideoItemPrivate::srcBig() const
{
    return _srcBig;
}

void VideoItemPrivate::setSrcBig(const QUrl &srcBig)
{
    if (_srcBig != srcBig)
    {
        _srcBig = srcBig;
        emitPropertyChanged("srcBig");
    }
}

QString VideoItemPrivate::accessKey() const
{
    return _accessKey;
}

void VideoItemPrivate::setAccessKey(const QString &accessKey)
{
    if (_accessKey != accessKey)
    {
        _accessKey = accessKey;
        emitPropertyChanged("accessKey");
    }
}

QUrl VideoItemPrivate::url() const
{
    QString url = QString("https://vk.com/video%1_%2")
            .arg(ownerId())
            .arg(id());

    return QUrl(url);
}
