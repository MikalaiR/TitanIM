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

QString AttachmentList::description() const
{
    if (ObservableCollection::count())
    {
        switch (at(0)->attachmentType()) {
        case Attachment::Photo:
        {
            return tr("Photo");
            break;
        }

        case Attachment::Video:
        {
            return tr("Video");
            break;
        }

        case Attachment::Audio:
        {
            return tr("Audio");
            break;
        }

        case Attachment::Doc:
        {
            return tr("Doc");
            break;
        }

        case Attachment::Sticker:
        {
            return tr("Sticker");
            break;
        }

        case Attachment::Map:
        {
            return tr("Location");
            break;
        }

        case Attachment::Fwd_messages:
        {
            return tr("Forward messages");
            break;
        }

        case Attachment::Gift:
        {
            return tr("Gift");
            break;
        }

        case Attachment::Wall:
        {
            return tr("Wall post");
            break;
        }

        case Attachment::Link:
        {
            return tr("Link");
            break;
        }

        default:
        {
            return tr("Attachment");
            break;
        }
        }
    }
    else
    {
        return "";
    }
}

QVariantList AttachmentList::filterByType(const int attachmentType) const
{
    QVariantList list;

    for (int i = 0; i < ObservableCollection::count(); i++)
    {
        if (at(i)->attachmentType() & attachmentType)
        {
            list.append(QVariant::fromValue(at(i).data()));
        }
    }

    return list;
}
