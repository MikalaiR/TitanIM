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
    id: chatDelegate
    width: chatDelegate.ListView.view.width
    height: Math.max(avatar.height + 4, bubble.height + 9) + sectionText.height

    SectionText {
        id: sectionText
        anchors.horizontalCenter: parent.horizontalCenter
        visible: chatDelegate.ListView.nextSection !== chatDelegate.ListView.section
        text: model.section
    }

    Item {
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        AvatarImage {
            id: avatar
            width: 32
            height: 32
            LayoutMirroring.enabled: model.isOut
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            source: model.decoration
            visible: model.decoration
        }

        BubbleItem {
            id: bubble
            LayoutMirroring.enabled: model.isOut
            maximumWidth: chatDelegate.width * 0.6
            anchors.left: avatar.right
            anchors.leftMargin: 3
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: -2
            isOut: model.isOut
            text: model.body
            time: model.timeStr
            attachments: model.attachments
        }
    }
}
