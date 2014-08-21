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

#include "stickerparser.h"

StickerItem StickerParser::parser(const QVariantMap &item)
{
    StickerItem sticker = StickerItem::create();

    int id = item.value("id").toInt();
    QUrl src = item.value("photo_128").toUrl();
    int width = item.value("width").toInt();
    int height = item.value("height").toInt();

    sticker->setId(id);
    sticker->setSrc(src);
    sticker->setWidth(width);
    sticker->setHeight(height);

    return sticker;
}
