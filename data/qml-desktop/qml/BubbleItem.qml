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
import QtGraphicalEffects 1.0

Item {
    id: bubbleItem

    property int maximumWidth
    property color color: "transparent"
    property var modelData

    implicitWidth: bubble.width
    implicitHeight: bubble.height

    BorderImage {
        id: bubble
        width: Math.max(body.contentWidth, attachmentsView.width) + 29
        height: (body.contentHeight + attachmentsView.height) + (body.height ? 8 : 6) +
                (attachmentsView.height && body.height ? 11 + content.spacing : 0)
        source: modelData.isOut ? "images/blue_bubble.sci" : "images/grey_bubble.sci"
        smooth: true
        visible: !modelData.isSticker
    }

    Loader {
        anchors.fill: bubble
        active: bubbleItem.color !== Qt.rgba(0, 0, 0, 0)

        sourceComponent: ColorOverlay {
            source: bubble
            color: bubbleItem.color
        }
    }

    Column {
        id: content
        anchors.left: bubble.left
        anchors.leftMargin: modelData.isOut ? 12 : 17
        anchors.verticalCenter: bubble.verticalCenter
        spacing: 4

        TextEditItem {
            id: body
            width: bubbleItem.maximumWidth
            height: text.length ? implicitHeight + 2: 0
            verticalAlignment: Text.AlignTop
            font.pointSize: main.fontPointSize - 1
            text: modelData.display
            wrapMode: Text.Wrap
            readOnly: true
            selectByMouse: true
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Loader {
            id: attachmentsView
            anchors.left: body.left
            active: modelData.attachments && modelData.attachments.count()

            sourceComponent: AttachmentsView {
                maximumWidth: bubbleItem.maximumWidth
                attachments: modelData.attachments
            }
        }
    }

    Text {
        LayoutMirroring.enabled: !modelData.isOut
        anchors.right: parent.left
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        font.pointSize: main.fontPointSize - 4
        color: modelData.isError ? "#860004" : "#8B92A4"
        text: modelData.isError ? qsTr("error") : (modelData.isSending ? qsTr("sending...") : modelData.timeStr)
    }

    Loader {
        anchors.fill: bubble
        active: modelData.isSingle

        sourceComponent: OpacityMask {
            source: attachmentsView
            maskSource: bubble
        }
    }
}
