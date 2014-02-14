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

#include "attachmentlist.h"

AttachmentList::AttachmentList()
{
}

int AttachmentList::count() const
{
    return ObservableCollection::count();
}

QVariantList AttachmentList::filterByType(const Attachment::AttachmentType type) const
{
    QVariantList list;

    for (int i = 0; i < count(); i++)
    {
        if (at(i)->attachmentType() == type)
        {
            list.append(QVariant::fromValue(at(i).data()));
        }
    }

    return list;
}
