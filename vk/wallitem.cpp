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

#include "wallitem.h"

WallItemPrivate::WallItemPrivate()
{
    setAttachmentType(Wall);
}

QString WallItemPrivate::text() const
{
    return _text;
}

void WallItemPrivate::setText(const QString &text)
{
    if (_text != text)
    {
        _text = text;
        emitPropertyChanged("text");
    }
}

QString WallItemPrivate::title() const
{
    return _text.left(50).replace("\n", " ") + "...";
}

QUrl WallItemPrivate::url() const
{
    QString url = QString("https://vk.com/wall%1_%2")
            .arg(ownerId())
            .arg(id());

    return QUrl(url);
}
