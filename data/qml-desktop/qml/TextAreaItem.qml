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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: textAreaItem

    property alias text: textArea.text
    property alias font: textArea.font
    property alias cursorPosition: textArea.cursorPosition
    property string placeholderText: ""
    property int maxLineCount: 7
    signal accepted

    function cursorPositionToEnd()
    {
        textArea.cursorPosition = textArea.length
    }

    TextArea {
        id: textArea

        property Component textAreaMacStyle: TextAreaStyle {
            decrementControl: Item {}
            incrementControl: Item {}
            handle: Item {}
            scrollBarBackground: Item {}

            frame: Item {
                anchors.fill: parent
                anchors.topMargin: -2
                anchors.bottomMargin: -2
                anchors.leftMargin: -6
                anchors.rightMargin: -7

                BorderImage {
                    border.bottom: 14
                    border.top: 13
                    border.right: 15
                    border.left: 15
                    anchors.fill: parent
                    source: "images/input_field.png"
                }

            }
        }

        height: parent.height - 4
        width: parent.width - 12
        anchors.centerIn: parent
        style: textAreaMacStyle
        backgroundVisible: false
        wrapMode: TextInput.Wrap

        onLineCountChanged: {
            textAreaItem.height = font.pixelSize * Math.min(lineCount + 1, maxLineCount + 1) + 2
        }

        Keys.onReturnPressed: {
            textAreaItem.accepted()
        }
    }

    Text {
        id: placeholder
        anchors.left: textArea.left
        anchors.leftMargin: 5
        anchors.verticalCenter: textArea.verticalCenter
        text: placeholderText
        color: "grey"
        font: textArea.font
        visible: !textArea.focus && !textArea.length
    }
}
