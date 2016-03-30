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
    id: audioFrame

    readonly property string name: "audio"

    Item {
        id: header
        height: 43
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Item {
            width: childrenRect.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            TextAreaItem {
                id: textArea
                anchors.left: parent.left
                anchors.right: cancelBtn.visible ? cancelBtn.left : parent.right
                anchors.rightMargin: cancelBtn.visible ? 10 : 0
                anchors.verticalCenter: parent.verticalCenter
                minimumHeight: 24
                font.family: "Helvetica"
                font.pixelSize: 12
                icon: "images/search.png"
                placeholderText: qsTr("Search music...")
                smile: false

                onAccepted: {
                    audio.search(textArea.getText())
                }

                onLengthChanged: {
                    if (!textArea.length) {
                        audio.refreshCurrentAlbum()
                    } else {
                        timerSendSearch.restart()
                    }
                }
            }

            ButtonItem {
                id: cancelBtn
                width: 100
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Cancel")
                visible: textArea.getText().length

                onClicked: {
                    textArea.text = ""
                }
            }

            Timer {
                id: timerSendSearch
                interval: 500
                onTriggered: {
                    if (textArea.getText()) {
                        audio.search(textArea.getText())
                    }
                }
            }

            Connections {
                target: audio
                onCurrentAlbumIdChanged: {
                    textArea.text = ""
                }
            }
        }
    }

    ListView {
        id: audioList
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 45
        model: audio.model
        delegate: AudioDelegate { }
        clip: true
    }
}
