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

Item {
    id: bubbleItem

    property int maximumWidth
    property bool isOut: false
    property alias text: body.text
    property var attachments

    implicitWidth: bubble.width
    implicitHeight: bubble.height

    BorderImage {
        id: bubble
        width: Math.max(body.contentWidth, attachmentsView.width) + 29
        height: (body.contentHeight + attachmentsView.height) + (body.height ? 12 : 8) +
                (attachmentsView.height && body.height ? 11 + content.spacing : 0)
        source: bubbleItem.isOut ? "images/blue_bubble.sci" : "images/grey_bubble.sci"
        smooth: true

        Column {
            id: content
            anchors.left: bubble.left
            anchors.leftMargin: bubbleItem.isOut ? 12 : 17
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -2
            spacing: 4

            Text {
                id: body
                width: bubbleItem.maximumWidth
                height: text.length ? implicitHeight + 2: 0
                verticalAlignment: Text.AlignTop
                font.pointSize: 13 - 1
                lineHeight: 0.9
                wrapMode: Text.Wrap
            }

            Loader {
                id: attachmentsView
                anchors.left: body.left
                active: bubbleItem.attachments && bubbleItem.attachments.count()

                sourceComponent: AttachmentsView {
                    maximumWidth: bubbleItem.maximumWidth
                    attachments: bubbleItem.attachments
                }
            }
        }
    }

    Loader {
        anchors.fill: bubble
        active: bubbleItem.attachments && !body.text.length &&
                bubbleItem.attachments.count() === 1 && attachmentsView.item.photoCount === 1

        sourceComponent: ImageMask {
            image: attachmentsView
            mask: bubble
            blendColor: "#DFE3EA"
        }
    }
}