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

import QtQuick 2.2

Item {
    id: underlineLabel

    property string label: ""
    property string text: ""
    property bool labelBig: false
    property bool select: false
    property color colorLabel: "#2B83DC"
    property color colorText: "black"
    property int spacing: 10

    signal clicked

    width: parent.width
    height: childrenRect.height + spacing

    Column {
        id: col
        anchors.top: parent.top
        spacing: 4

        Text {
            text: underlineLabel.label
            color: underlineLabel.colorLabel
            font.pointSize: labelBig ? main.fontPointSize : main.fontPointSize - 1
        }

        TextEdit {
            readOnly: true
            selectByMouse: true
            persistentSelection: true
            text: underlineLabel.text
            font.pointSize: labelBig ? main.fontPointSize - 1 : main.fontPointSize
            color: underlineLabel.colorText
            selectionColor: "#58a4e6"

            Loader {
                anchors.fill: parent
                anchors.margins: -5
                active: !underlineLabel.select

                sourceComponent: MouseArea {
                    onClicked: {
                        underlineLabel.clicked()
                    }
                }
            }
        }
    }

    Rectangle {
        height: 1
        width: parent.width
        anchors.top: col.bottom
        anchors.topMargin: 9
        anchors.left: col.left
        color: "#e8e8e8"
    }
}
