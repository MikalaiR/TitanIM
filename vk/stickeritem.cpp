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

#include "stickeritem.h"

StickerItemPrivate::StickerItemPrivate()
{
    setAttachmentType(Sticker);

    _width = 0;
    _height = 0;
}

QUrl StickerItemPrivate::src() const
{
    return _src;
}

void StickerItemPrivate::setSrc(const QUrl &src)
{
    if (_src != src)
    {
        _src = src;
        emitPropertyChanged("src");
    }
}

int StickerItemPrivate::width() const
{
    return _width;
}

void StickerItemPrivate::setWidth(const int width)
{
    if (_width != width)
    {
        _width = width;
        emitPropertyChanged("width");
    }
}

int StickerItemPrivate::height() const
{
    return _height;
}

void StickerItemPrivate::setHeight(const int height)
{
    if (_height != height)
    {
        _height = height;
        emitPropertyChanged("height");
    }
}
