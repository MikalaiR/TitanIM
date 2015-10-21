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

#ifndef PHOTOSPACKET_H
#define PHOTOSPACKET_H

#include <QObject>
#include "connection.h"
#include "photoparser.h"

class PhotosPacket : public QObject
{
    Q_OBJECT

public:
    PhotosPacket(Connection *connection);

private:
    Connection *_connection;

public slots:
    void getPhotos(const int id, const QString &albumId);

protected slots:
    void getPhotosFinished(const Packet *sender, const QVariantMap &result);

signals:
    void photos(const int id, const QList<PhotoItem> &photos);
};

#endif // PHOTOSPACKET_H
