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

#ifndef MAPITEM_H
#define MAPITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class MapItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString latitude READ latitude CONSTANT) //todo signal
    Q_PROPERTY(QString longitude READ longitude CONSTANT)
    Q_PROPERTY(QUrl src READ src CONSTANT)
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    MapItemPrivate();
    QString latitude() const;
    void setLatitude(const QString &latitude);
    QString longitude() const;
    void setLongitude(const QString &longitude);
    QUrl src() const;
    int width() const;
    int height() const;
    QUrl url() const;

private:
    QString _latitude;
    QString _longitude;
    int _width;
    int _height;
};

typedef QSharedPointer<MapItemPrivate> MapItem;

#endif // MAPITEM_H
