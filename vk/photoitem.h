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

#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class PhotoItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QUrl src READ src CONSTANT) //todo signal
    Q_PROPERTY(QUrl srcBig READ srcBig CONSTANT)
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)

public:
    PhotoItemPrivate();
    int albumId() const;
    void setAlbumId(const int albumId);
    QUrl src() const;
    void setSrc(const QUrl &src);
    QUrl srcBig() const;
    void setSrcBig(const QUrl &srcBig);
    int width() const;
    void setWidth(const int width);
    int height() const;
    void setHeight(const int height);

private:
    int _albumId;
    QUrl _src;
    QUrl _srcBig;
    int _width;
    int _height;
};

typedef QSharedPointer<PhotoItemPrivate> PhotoItem;

#endif // PHOTOITEM_H
