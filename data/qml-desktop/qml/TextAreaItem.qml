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
import TitanIM.Tool 1.0

Item {
    id: textAreaItem

    property alias text: textArea.text
    property alias font: textArea.font
    property alias cursorPosition: textArea.cursorPosition
    property string placeholderText: ""
    property int minimumHeight: 26
    property int maximumHeight: 91
    property bool smile: true
    signal accepted

    function cursorPositionToEnd() {
        textArea.cursorPosition = textArea.length
    }

    function getText() {
        return textArea.getFormattedText(0, textArea.length)
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
                anchors.leftMargin: -6
                anchors.rightMargin: smile ? -emoticonsButton.width : 0

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

        height: parent.height
        width: parent.width -(emoticonsButton.width + 6)
        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        style: textAreaMacStyle
        verticalAlignment: TextEdit.AlignVCenter
        backgroundVisible: false
        textFormat: TextEdit.RichText
        wrapMode: TextInput.Wrap

        onTextChanged: {
            if (lineCount > 1) {
                textAreaItem.height = Math.min(flickableItem.contentHeight + 4, maximumHeight)
            } else {
                textAreaItem.height = minimumHeight
            }
        }

        Keys.onPressed: {
            if (event.modifiers !== Qt.NoModifier) {
                return
            }

            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                    textAreaItem.accepted()
                    event.accepted = true
            }

            if (event.key === Qt.Key_Tab) {
                EmoticonsBox.show(emoticonsButton, textArea)
                event.accepted = true
            }
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

    Image {
        id: emoticonsButton
        width: visible ? implicitWidth : 0
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        source: "images/emoticons_button.png"
        visible: smile

        MouseArea {
            anchors.fill: parent
            onPressed: {
                EmoticonsBox.show(emoticonsButton, textArea)
            }
        }
    }
}
