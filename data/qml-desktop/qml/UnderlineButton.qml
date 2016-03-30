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
    id: underlineButton

    property string label: ""
    property color color: "#2B83DC"

    signal clicked

    width: parent.width
    height: childrenRect.height + 10

    Text {
        id: labelText
        anchors.top: parent.top
        text: underlineButton.label
        color: enabled ? underlineButton.color : "grey"
        font.pointSize: main.fontPointSize + 1

        MouseArea {
            anchors.fill: parent
            anchors.margins: -5
            onClicked: {
                underlineButton.clicked()
            }
        }
    }

    Rectangle {
        height: 1
        width: parent.width
        anchors.top: labelText.bottom
        anchors.topMargin: 9
        anchors.left: labelText.left
        color: "#e8e8e8"
    }
}
