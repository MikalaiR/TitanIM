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
import TitanIM.Viewer 1.0

Item {
    id: profile

    readonly property string name: "profile"
    property var profileItem: engine.getUser(chats.currentChatId)

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    Flickable {
        id: flickableItem
        anchors.fill: parent
        anchors.topMargin: 30
        contentWidth: content.width
        contentHeight: content.height
        visible: chats.currentChatId !== 0

        MouseArea {
            anchors.fill: parent
            anchors.topMargin: -30

            onClicked: {
                mainWindow.popPage()
            }
        }

        Item {
            id: content
            width: profile.width
            height: childrenRect.height + 20

            Avatar {
                id: avatar
                width: 70
                height: 70
                anchors.left: parent.left
                anchors.leftMargin: 80
                anchors.top: parent.top
                source: chats.currentChatDialog.decoration
                radius: 40

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        PhotoViewer.setUrl(profileItem.photoMediumRect)
                    }
                }
            }

            Column {
                id: infoColumn
                anchors.verticalCenter: avatar.verticalCenter
                anchors.left: avatar.right
                anchors.leftMargin: 20
                spacing: 3

                Text {
                    id: name
                    width: 100
                    font.pointSize: main.fontPointSize + 1
                    text: chats.currentChatDialog.displayName
                }

                Text {
                    id: status
                    color: "grey"
                    font.pointSize: main.fontPointSize - 2
                    text: chats.currentChatDialog.description
                }
            }

            Column {
                id: globalColumn1
                anchors.left: avatar.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: avatar.bottom
                anchors.topMargin: 45
                spacing: 6

                UnderlineLabel {
                    visible: text.length
                    label: "город"
                    text: profileItem.city
                    select: true
                }

                UnderlineLabel {
                    visible: text.length
                    label: "день рождения"
                    text: profileItem.bdate
                    select: true
                }

                UnderlineLabel {
                    visible: text.length
                    label: "мобильный"
                    text: profileItem.mobilePhone
                    select: true
                }

                UnderlineLabel {
                    visible: profileItem.domain.length
                    label: "ссылка"
                    text: "https://vk.com/" + profileItem.domain

                    onClicked: {
                        Qt.openUrlExternally(text);
                    }
                }
            }

            Column {
                id: globalColumn2
                anchors.left: avatar.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: globalColumn1.bottom
                anchors.topMargin: 20
                spacing: 6

                UnderlineButton {
                    label: "Отправить сообщение"

                    onClicked: {
                        mainWindow.popPage({item: Qt.resolvedUrl("Chat.qml")})
                    }
                }

                UnderlineButton {
                    label: "Заблокировать"
                }
            }
        }
    }
}

