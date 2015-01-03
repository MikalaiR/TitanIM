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

#include "giftitem.h"

GiftItemPrivate::GiftItemPrivate()
{
    setAttachmentType(Gift);
}

QUrl GiftItemPrivate::src() const
{
    return _src;
}

void GiftItemPrivate::setSrc(const QUrl &src)
{
    if (_src != src)
    {
        _src = src;
        emitPropertyChanged("src");
    }
}

QUrl GiftItemPrivate::srcBig() const
{
    return _srcBig;
}

void GiftItemPrivate::setSrcBig(const QUrl &srcBig)
{
    if (_srcBig != srcBig)
    {
        _srcBig = srcBig;
        emitPropertyChanged("srcBig");
    }
}
