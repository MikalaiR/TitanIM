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

    QMetaEnum metaEnum = Attachment::metaEnumerator("AttachmentType");
    int type;

    for (QVariant item: items)
    {
        QString strType = item.toMap()["type"].toString();
        QVariantMap attachment = item.toMap()[strType].toMap();

        type = metaEnum.keyToValue(Utils::firstUpper(strType).toLatin1());

        switch (type) {
        case Attachment::Photo:
        {
            PhotoItem photo = PhotoParser::parser(attachment);
            attachments->add(photo);
            break;
        }

        case Attachment::Sticker:
        {
            StickerItem sticker = StickerParser::parser(attachment);
            attachments->add(sticker);
            break;
        }

        case Attachment::Audio:
        {
            AudioItem audio = AudioParser::parser(attachment);
            attachments->add(audio);
            break;
        }

        case Attachment::Video:
        {
            VideoItem video = VideoParser::parser(attachment);
            attachments->add(video);
            break;
        }

        case Attachment::Doc:
        {
            DocItem doc = DocParser::parser(attachment);
            attachments->add(doc);
            break;
        }

        case Attachment::Gift:
        {
            GiftItem gift = GiftParser::parser(attachment);
            attachments->add(gift);
            break;
        }

        case Attachment::Wall:
        {
            WallItem wall = WallParser::parser(attachment);
            attachments->add(wall);
            break;
        }

        case Attachment::Link:
        {
            LinkItem link = LinkParser::parser(attachment);
            attachments->add(link);
            break;
        }

        default:
        {
            qWarning() << "AttachmentsParser: unknown attachment" << strType;
            break;
        }
        }
    }

    return attachments;
}
