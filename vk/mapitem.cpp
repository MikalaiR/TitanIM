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

#include "mapitem.h"

MapItemPrivate::MapItemPrivate()
{
    setAttachmentType(Map);

    _width = 250;
    _height = 130;
}

QString MapItemPrivate::latitude() const
{
    return _latitude;
}

void MapItemPrivate::setLatitude(const QString &latitude)
{
    if (_latitude != latitude)
    {
        _latitude = latitude;
        emitPropertyChanged("latitude");
        emitPropertyChanged("src");
    }
}

QString MapItemPrivate::longitude() const
{
    return _longitude;
}

void MapItemPrivate::setLongitude(const QString &longitude)
{
    if (_longitude != longitude)
    {
        _longitude = longitude;
        emitPropertyChanged("longitude");
        emitPropertyChanged("src");
    }
}

QUrl MapItemPrivate::src() const
{
    return QString("http://maps.googleapis.com/maps/api/staticmap?zoom=15&size=%1x%2&sensor=false&center=%3,%4")
            .arg(_width)
            .arg(_height)
            .arg(_latitude)
            .arg(_longitude);
}

int MapItemPrivate::width() const
{
    return _width;
}

int MapItemPrivate::height() const
{
    return _height;
}

QUrl MapItemPrivate::url() const
{
    return QString("http://www.google.com/maps?q=@%1,%2").arg(_latitude).arg(_longitude);
}
