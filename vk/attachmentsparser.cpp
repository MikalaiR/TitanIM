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

#include "attachmentsparser.h"

AttachmentList* AttachmentsParser::parser(const QVariantList &items)
{
    AttachmentList *attachments = new AttachmentList();

    const QMetaObject &metaObject = Attachment::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("AttachmentType"));

    Attachment::AttachmentType type;

    foreach (QVariant item, items)
    {
        QString strType = item.toMap()["type"].toString();
        QVariantMap attachment = item.toMap()[strType].toMap();

        type = (Attachment::AttachmentType)metaEnum.keyToValue(strType.toUtf8());

        switch (type) {
        case Attachment::photo:
        {
            PhotoItem photo = PhotoParser::parser(attachment);
            attachments->add(photo);
            break;
        }

        case Attachment::unknown:
        {
            qWarning() << "AttachmentsParser: unknown attachment" << strType;
            break;
        }

        default:
        {
            qWarning() << "AttachmentsParser: not implemented parser" << strType;
            break;
        }
        }
    }

    return attachments;
}
