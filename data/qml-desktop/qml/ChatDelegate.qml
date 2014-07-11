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
    id: chatDelegate
    width: chatDelegate.ListView.view.width
    height: Math.max(avatar.height + 4, bubble.height + 9) + sectionText.height

    Rectangle {
        id: unreadHighlight
        anchors.top: body.top
        anchors.bottom: body.bottom
        anchors.left: chatDelegate.left
        anchors.right: chatDelegate.right
        color: chatFrame.unreadHighlightColor
        visible: model.messageType === MessageBase.Text && model.isUnread
    }

    Loader {
        id: sectionText
        anchors.horizontalCenter: parent.horizontalCenter
        active: model.messageType !== MessageBase.Typing

        sourceComponent: SectionText {
            visible: chatDelegate.ListView.nextSection !== chatDelegate.ListView.section
            text: model.section
        }
    }

    Item {
        id: body
        anchors.top: sectionText.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        AvatarImage {
            id: avatar
            width: 32
            height: 32
            LayoutMirroring.enabled: model.messageType === MessageBase.Text && model.isOut
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            source: model.decoration
            visible: model.decoration
        }

        Loader {
            id: typing
            anchors.left: avatar.right
            anchors.leftMargin: 4
            anchors.bottom: avatar.bottom
            active: model.messageType === MessageBase.Typing

            sourceComponent: Image {
                height: 24;
                fillMode: Image.PreserveAspectFit
                source: "images/typing.png"
            }
        }

        Loader {
            id: bubble
            anchors.left: avatar.right
            anchors.leftMargin: 3
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: -2
            LayoutMirroring.enabled: model.isOut
            active: model.messageType === MessageBase.Text

            sourceComponent: BubbleItem {
                maximumWidth: chatDelegate.width * 0.6
                isOut: model.isOut
                isUnread: model.isUnread
                isSending: model.isSending
                isError: model.isError
                text: model.body
                time: model.timeStr
                attachments: model.attachments
            }
        }
    }
}
