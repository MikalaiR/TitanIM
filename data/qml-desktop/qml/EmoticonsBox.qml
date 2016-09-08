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

pragma Singleton

import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.2

Item {
    id: emoticonsBox
    parent: main.contentItem

    readonly property alias isVisible: window.visible
    property int currentEmojiBlock: 1
    property var __text

    function show(item, text) {
        __text = text

        var i = item.mapToItem(null, 0, 0)
        var g = main.mapToGlobal(i.x, i.y)

        window.setX(g.x - window.width + 43)
        window.setY(g.y - window.height - 1)

        window.show()
        window.requestActivate()
    }

    function hide() {
        window.hide()
    }

    function openBlock(block) {
        grid.model.clear()
        emoticonsBox.currentEmojiBlock = block

        var emojis = emoticons.emojiList(block)
        for (var i = 0; i < emojis.length; i++) {
            grid.model.append({"code": emojis[i]})
        }

        grid.positionViewAtBeginning()
    }

    Window {
        id: window
        width: 273
        height: 255
        maximumWidth: width
        maximumHeight: height
        minimumWidth: width
        minimumHeight: height
        flags: Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint | Qt.ToolTip
        visible: box.activeFocus
        color: "transparent"

        BorderImage {
            id: borderImage
            anchors.fill: parent
            border { left: 4; top: 4; right: 45; bottom: 15 }
            source: "images/emoticons_popover.png"
        }

        Item {
            id: box
            anchors.fill: borderImage
            anchors.margins: 1
            anchors.bottomMargin: rowEmojiBlock.height + 13
            clip: true
            focus: true

            GridView {
                id: grid
                anchors.fill: parent
                anchors.margins: 5
                anchors.bottomMargin: 0
                model: ListModel{}
                cellWidth: 26
                cellHeight: 26
                delegate: Rectangle {
                    width: grid.cellWidth
                    height: grid.cellHeight
                    radius: 7
                    color: mouseArea.pressed ? "#dbd8d8" : mouseArea.containsMouse ? "#e8e7e7" : "transparent"

                    Image {
                        id: img
                        anchors.centerIn: parent
                        source: emoticons.pathOf(code)
                        smooth: true
                        fillMode: Image.PreserveAspectFit
                        asynchronous: true
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            __text.insert(__text.cursorPosition, "<img src=\"%1\"/> ".arg(img.source))
                            emoticons.addRecent(code)
                        }
                    }
                }

                Component.onCompleted: emoticonsBox.openBlock(1)
            }

            Keys.onPressed: {
                if (event.modifiers !== Qt.NoModifier) {
                    return
                }

                if (event.key === Qt.Key_Tab || event.key === Qt.Key_Escape) {
                    emoticonsBox.hide()
                    event.accepted = true
                }
            }
        }

        Row {
            id: rowEmojiBlock
            height: 30
            anchors.left: box.left
            anchors.leftMargin: 7
            anchors.right: box.right
            anchors.rightMargin: 7
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 7
            spacing: 10

            Repeater {
                model: 6

                Image {
                    source: emoticonsBox.currentEmojiBlock === modelData ?
                                "images/emoji_block%1_selected.png".arg(modelData + 1) :
                                "images/emoji_block%1.png".arg(modelData + 1)

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            emoticonsBox.openBlock(modelData)
                        }
                    }
                }
            }
        }
    }
}
