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
#include "observablecollection.h"
#include "attachmentitem.h"

class AttachmentListPrivate : public ObservableCollection<AttachmentItem>
{
    Q_OBJECT

public:
    AttachmentListPrivate();
};


typedef QSharedPointer<AttachmentListPrivate> AttachmentList;

#endif // ATTACHMENTLIST_H
