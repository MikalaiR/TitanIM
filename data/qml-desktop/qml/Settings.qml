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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

Item {
    anchors.fill: parent

    function refresh() {
        loader.active = false
        loader.active = true
    }

    Loader {
        id: loader
        anchors.fill: parent
        active: false
        sourceComponent: Item {
            property var profile: engine.getUser()

            AvatarImage {
                id: avatar
                width: 70
                height: 70
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: -name.width + 50
                anchors.top: parent.top
                anchors.topMargin: 20
                source: profile.photoMediumRect
                radius: 40
            }

            Column {
                id: infoColumn
                anchors.verticalCenter: avatar.verticalCenter
                anchors.left: avatar.right
                anchors.leftMargin: 20
                spacing: 7

                Text {
                    id: name
                    font.pointSize: main.fontPointSize + 2
                    text: profile.fullName
                }

                Text {
                    id: status
                    color: "grey"
                    font.pointSize: main.fontPointSize
                    text: profile.online ? qsTr("online") : qsTr("offline")
                    visible: name.text.length > 1
                }
            }

            Column {
                id: globalCol
                width: Math.min(parent.width - 60, 300)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: avatar.bottom
                anchors.topMargin: 60
                spacing: 5

                UnderlineToggleButton {
                    id: keepOnline
                    label: qsTr("Keep online")
                    on: settings.loadBoolProfile("main/keepOnline", true)
                    color: "black"

                    onClickedToggle: {
                        settings.saveProfile("main/keepOnline", keepOnline.on)
                        authorization.keepOnline(keepOnline.checked)
                    }
                }

                UnderlineToggleButton {
                    id: autoRead
                    label: qsTr("Auto read message")
                    on: settings.loadBoolProfile("chat/autoRead", true)
                    color: "black"

                    onClickedToggle: {
                        settings.saveProfile("chat/autoRead", autoRead.on)
                    }
                }
            }

            ButtonItem {
                width: Math.min(parent.width - 60, 230)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: globalCol.bottom
                anchors.topMargin: 30
                text: qsTr("Log Out")

                onClicked: {
                    authorization.disconnectVk()
                }
            }
        }
    }
}
