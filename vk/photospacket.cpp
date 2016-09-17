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

#include "photospacket.h"

PhotosPacket::PhotosPacket(Connection *connection)
{
    _connection = connection;
}

void PhotosPacket::getPhotos(const int id, const QString &albumId)
{
    Packet *packet = new Packet("photos.get");
    packet->setId(id);
    packet->addParam("owner_id", id);
    packet->addParam("album_id", albumId);
    packet->addParam("rev", 1);
    connect(packet, &Packet::finished, this, &PhotosPacket::getPhotosFinished);
    _connection->appendQuery(packet);
}

void PhotosPacket::getPhotosFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList items = response.value("items").toList();

    QList<PhotoItem> list;

    foreach (QVariant item, items)
    {
        PhotoItem photo = PhotoParser::parser(item.toMap());
        list.append(photo);
    }

    emit photos(sender->id(), list);
    delete sender;
}

