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
    id: linkAttachments

    implicitWidth: content.width
    implicitHeight: content.height

    Row {
        id: content
        width: Math.max(title.contentWidth, description.contentWidth) + img.width + 10
        height: childrenRect.height
        spacing: 7

        Image {
            id: img
            width: 30
            height: 30
            source: "images/attach_link.png"
        }

        Column {
            width: Math.min(maximumWidth, 240) - img.width
            height: childrenRect.height
            anchors.verticalCenter: img.verticalCenter

            Text {
                id: title
                anchors.left: parent.left
                width: parent.width
                color: "black"
                font.pointSize: main.fontPointSize
                elide: Text.ElideRight
                text: items[0].title
            }

            Text {
                id: description
                anchors.left: parent.left
                width: parent.width
                color: "grey"
                font.pointSize: main.fontPointSize - 3
                elide: Text.ElideRight
                text: items[0].url
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(items[0].url)
        }
    }
}
