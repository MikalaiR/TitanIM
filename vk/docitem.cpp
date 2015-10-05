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

#include "docitem.h"

DocItemPrivate::DocItemPrivate()
{
    setAttachmentType(Doc);

    _size = 0;
}

QString DocItemPrivate::title() const
{
    return _title;
}

void DocItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

qint64 DocItemPrivate::size() const
{
    return _size;
}

QString DocItemPrivate::sizeStr() const
{
    return _sizeStr;
}

void DocItemPrivate::setSize(const qint64 size)
{
    if (_size != size)
    {
        _size = size;
        _sizeStr = Utils::fileSizeToText(_size);
        emitPropertyChanged("size");
    }
}

QString DocItemPrivate::ext() const
{
    return _ext;
}

void DocItemPrivate::setExt(const QString &ext)
{
    if (_ext != ext)
    {
        _ext = ext;
        emitPropertyChanged("ext");
    }
}

QUrl DocItemPrivate::url() const
{
    return _url;
}

void DocItemPrivate::setUrl(const QUrl &url)
{
    if (_url != url)
    {
        _url = url;
        emitPropertyChanged("url");
    }
}

QUrl DocItemPrivate::thumb() const
{
    return _thumb;
}

void DocItemPrivate::setThumb(const QUrl &thumb)
{
    if (_thumb != thumb)
    {
        _thumb = thumb;
        emitPropertyChanged("thumb");
    }
}

bool DocItemPrivate::isImage() const
{
    return !_thumb.isEmpty();
}
