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

#ifndef ATTACHMENTLIST_H
#define ATTACHMENTLIST_H

#include <QSharedPointer>
#include <QVariant>
#include "observablecollection.h"
#include "attachmentitem.h"

class AttachmentList : public ObservableCollection<AttachmentItem>
{
    Q_OBJECT

public:
    AttachmentList();
    Q_INVOKABLE int count() const;
    QString description() const;

public slots:
    QVariantList filterByType(const int attachmentType, const bool sharedPointer=false) const;
};

#endif // ATTACHMENTLIST_H
