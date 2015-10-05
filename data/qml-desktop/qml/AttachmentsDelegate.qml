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
    id: attachmentDelegate
    width: attachmentDelegate.ListView.view.height
    height: width - 2

    Image {
        id: img
        width: parent.width - 9
        height: parent.height - 9
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        source: model.decoration
        smooth: true
    }

    Text {
        anchors.left: img.left
        anchors.leftMargin: 5
        anchors.right: img.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        color: "white"
        font.pointSize: main.fontPointSize - 2
        style: Text.Outline
        styleColor: "black"
        text: model.display
    }

    Image {
        width: 22
        height: 22
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.right: parent.right
        source: "images/delete_attach.png"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                chats.currentChatAttachments.remove(index)
            }
        }
    }
}
