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

#ifndef GIFTITEM_H
#define GIFTITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class GiftItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QUrl src READ src CONSTANT) //todo signal
    Q_PROPERTY(QUrl srcBig READ srcBig CONSTANT)

public:
    GiftItemPrivate();
    QUrl src() const;
    void setSrc(const QUrl &src);
    QUrl srcBig() const;
    void setSrcBig(const QUrl &srcBig);

private:
    QUrl _src;
    QUrl _srcBig;
};

typedef QSharedPointer<GiftItemPrivate> GiftItem;

#endif // GIFTITEM_H
