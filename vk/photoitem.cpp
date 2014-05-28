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

#include "photoitem.h"

PhotoItemPrivate::PhotoItemPrivate()
{
    setAttachmentType(photo);

    _albumId = 0;
    _width = 0;
    _height = 0;
}

int PhotoItemPrivate::albumId() const
{
    return _albumId;
}

void PhotoItemPrivate::setAlbumId(const int albumId)
{
    if (_albumId != albumId)
    {
        _albumId = albumId;
        emitPropertyChanged("albumId");
    }
}

QUrl PhotoItemPrivate::src() const
{
    return _src;
}

void PhotoItemPrivate::setSrc(const QUrl &src)
{
    if (_src != src)
    {
        _src = src;
        emitPropertyChanged("src");
    }
}

QUrl PhotoItemPrivate::srcBig() const
{
    return _srcBig;
}

void PhotoItemPrivate::setSrcBig(const QUrl &srcBig)
{
    if (_srcBig != srcBig)
    {
        _srcBig = srcBig;
        emitPropertyChanged("srcBig");
    }
}

int PhotoItemPrivate::width() const
{
    return _width;
}

void PhotoItemPrivate::setWidth(const int width)
{
    if (_width != width)
    {
        _width = width;
        emitPropertyChanged("width");
    }
}

int PhotoItemPrivate::height() const
{
    return _height;
}

void PhotoItemPrivate::setHeight(const int height)
{
    if (_height != height)
    {
        _height = height;
        emitPropertyChanged("height");
    }
}
