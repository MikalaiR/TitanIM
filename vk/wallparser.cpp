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

#include "wallparser.h"

WallItem WallParser::parser(const QVariantMap &item)
{
    WallItem wall = WallItem::create();

    int id = item.value("id").toInt();
    int ownerId = item.contains("owner_id") ? item.value("owner_id").toInt() : item.value("to_id").toInt();
    QString text = item.value("text").toString();

    wall->setId(id);
    wall->setOwnerId(ownerId);
    wall->setText(text);

    return wall;
}
