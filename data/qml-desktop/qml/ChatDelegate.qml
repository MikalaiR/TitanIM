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
    height: bubble.height + 9

    Row {
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        spacing: 3
        LayoutMirroring.enabled: model.isOut

        AvatarImage {
            id: avatar
            width: 32
            height: 32
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            source: model.decoration
            visible: model.decoration
        }

        BubbleItem {
            id: bubble
            maximumWidth: chatDelegate.width * 0.6
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: -2
            isOut: model.isOut
            text: model.body
            attachments: model.attachments
        }
    }
}
