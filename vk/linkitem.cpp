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

#include "linkitem.h"

LinkItemPrivate::LinkItemPrivate()
{
    setAttachmentType(Link);
}

QString LinkItemPrivate::title() const
{
    return _title;
}

void LinkItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

QString LinkItemPrivate::description() const
{
    return _description;
}

void LinkItemPrivate::setDescription(const QString &description)
{
    if (_description != description)
    {
        _description = description;
        emitPropertyChanged("description");
    }
}

QUrl LinkItemPrivate::url() const
{
    return _url;
}

void LinkItemPrivate::setUrl(const QUrl &url)
{
    if (_url != url)
    {
        _url = url;
        emitPropertyChanged("url");
    }
}

