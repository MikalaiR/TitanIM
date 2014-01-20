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

#ifndef ATTACHMENTITEM_H
#define ATTACHMENTITEM_H

#include <QObject>
#include <QSharedPointer>
#include "notifypropertybase.h"

class AttachmentItemPrivate : public NotifyPropertyBase
{
    Q_OBJECT
    Q_ENUMS(AttachmentType)

public:
    enum AttachmentType
    {
        photo,
        video,
        audio,
        doc,
        map,
        fwd_messages,
        unknown = -1
    }; //VK types in lower case. QMetaObject

    AttachmentItemPrivate();
    AttachmentType attachmentType() const;

protected:
    void setAttachmentType(const AttachmentType attachmentType);

private:
    AttachmentType _attachmentType;
};

typedef QSharedPointer<AttachmentItemPrivate> AttachmentItem;

#endif // ATTACHMENTITEM_H
