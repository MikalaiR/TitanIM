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

#include "giftparser.h"

GiftItem GiftParser::parser(const QVariantMap &item)
{
    GiftItem gift = GiftItem::create();

    int id = item.value("id").toInt();
    QUrl src = item.value("thumb_96").toUrl();
    QUrl srcBig = item.value("thumb_256").toUrl();

    gift->setId(id);
    gift->setSrc(src);
    gift->setSrcBig(srcBig);

    return gift;
}
