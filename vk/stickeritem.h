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

#ifndef STICKERITEM_H
#define STICKERITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class StickerItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QUrl src READ src CONSTANT) //todo signal
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)

public:
    StickerItemPrivate();
    QUrl src() const;
    void setSrc(const QUrl &src);
    int width() const;
    void setWidth(const int width);
    int height() const;
    void setHeight(const int height);

private:
    QUrl _src;
    int _width;
    int _height;
};

typedef QSharedPointer<StickerItemPrivate> StickerItem;

#endif // STICKERITEM_H
