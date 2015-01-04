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

#ifndef WALLITEM_H
#define WALLITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class WallItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text CONSTANT) //todo
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    WallItemPrivate();
    QString text() const;
    void setText(const QString &text);
    QString title() const;
    QUrl url() const;

private:
    QString _text;
};

typedef QSharedPointer<WallItemPrivate> WallItem;

#endif // WALLITEM_H
