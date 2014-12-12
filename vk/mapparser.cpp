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

#include "mapparser.h"

MapItem MapParser::parser(const QVariantMap &item)
{
    MapItem map = MapItem::create();

    QStringList coordinates = item.value("coordinates").toString().split(" ");

    if (coordinates.count())
    {
        QString latitude = coordinates.at(0);
        QString longitude = coordinates.at(1);

        map->setLatitude(latitude);
        map->setLongitude(longitude);
    }

    return map;
}
