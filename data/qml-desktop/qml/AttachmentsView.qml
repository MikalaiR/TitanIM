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

import QtQuick 2.0
import TitanIM 2.0

Item {
    id: attachmentsView

    property int maximumWidth
    property var attachments
    readonly property int photoCount: attachments.filterByType(Attachment.Photo).length //todo remove

    property var __attachments: [
        {"items": attachments.filterByType(Attachment.Photo | Attachment.Video), "source": "PhotoVideoAttachments.qml"},
        {"items": attachments.filterByType(Attachment.Sticker), "source": "StickerAttachments.qml"},
        {"items": attachments.filterByType(Attachment.Map), "source": "MapAttachments.qml"},
        {"items": attachments.filterByType(Attachment.Audio), "source": "AudioAttachments.qml"},
        {"items": attachments.filterByType(Attachment.Fwd_messages), "source": "FwdMsgAttachments.qml"}
    ]

    implicitWidth: attachmentsFrame.width
    implicitHeight: attachmentsFrame.height

    Column {
        id: attachmentsFrame
        spacing: 5

        Repeater {
            model: __attachments

            Loader {
                property int maximumWidth: attachmentsView.maximumWidth
                property var items: modelData.items
                active: modelData.items.length
                source: modelData.source
            }
        }
    }
}
