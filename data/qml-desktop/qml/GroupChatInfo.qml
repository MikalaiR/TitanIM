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
import TitanIM 2.0

Item {
    id: groupChatInfo

    readonly property string name: "groupChatInfo"
    property int chatId: chats.currentChatId
    property var dialog: chats.currentChatDialog

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
        visible: chatId !== 0

        MouseArea {
            anchors.fill: parent
            anchors.topMargin: -30

            onClicked: {
                mainWindow.popPage()
            }
        }

        Item {
            id: content
            width: groupChatInfo.width
            height: childrenRect.height + 20

            Avatar {
                id: cover
                width: 70
                height: 70
                anchors.left: parent.left
                anchors.leftMargin: 80
                anchors.top: parent.top
                source: dialog.decoration
                radius: 40
            }

            Column {
                id: infoColumn
                anchors.verticalCenter: cover.verticalCenter
                anchors.left: cover.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                spacing: 3

                Text {
                    id: name
                    width: parent.width
                    font.pointSize: main.fontPointSize + 1
                    text: dialog.displayName
                    elide: Text.ElideRight
                }

                Text {
                    id: peopleConversation
                    color: "grey"
                    font.pointSize: main.fontPointSize - 2
                    text: dialog.description
                }
            }

            Column {
                id: globalColumn1
                anchors.left: cover.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: cover.bottom
                anchors.topMargin: 45
                visible: dialog.isGroupChat
                spacing: 6

                Repeater {
                    model: dialog.isGroupChat ? dialog.groupChatHandler.users : 0

                    Item {
                        height: Math.max(avatar.height, profile.height)
                        width: parent.width - avatar.width - 11

                        Row {
                            anchors.fill: parent
                            spacing: 10

                            AvatarImage {
                                id: avatar
                                width: 35
                                height: 35
                                anchors.verticalCenter: profile.verticalCenter
                                anchors.verticalCenterOffset: -5
                                source: modelData.photoMediumRect
                                radius: 30
                                online: modelData.online
                            }

                            UnderlineLabel {
                                id: profile
                                label: modelData.fullName
                                text: modelData.lastSeenText
                                labelBig: true
                                colorLabel: dialog.groupChatHandler.adminId === modelData.id ? "#408000" : "black"
                                colorText: "grey"
                                spacing: 5
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                main.openChat(modelData.id)
                            }
                        }
                    }
                }

                UnderlineButton {
                    label: qsTr("Add a person")
                    enabled: dialog.isGroupChat && dialog.groupChatHandler.usersCount !== 0

                    onClicked: {
                        chats.isSelectUser = true
                        mainWindow.tabBarCurrentIndex = 1
                    }
                }
            }

            Column {
                id: globalColumn2
                anchors.left: cover.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: globalColumn1.bottom
                anchors.topMargin: 15
                visible: dialog.isGroupChat
                spacing: 6

                UnderlineToggleButton {
                    id: toggleBtn
                    label: qsTr("Notifications")
                    on: !chats.isMuteUser(chatId)

                    onClickedToggle: {
                        chats.setMuteUser(chatId, !toggleBtn.on)
                    }
                }

                UnderlineButton {
                    label: qsTr("Leave conversation")
                    enabled: dialog.isGroupChat && dialog.groupChatHandler.usersCount !== 0
                    color: "red"

                    onClicked: {
                        dialog.groupChatHandler.removeChatUser()
                        mainWindow.popPage()
                    }
                }
            }
        }
    }

    Loader {
        id: forwardFrame
        z: 1
        anchors.fill: parent
        active: chats.isSelectUser

        sourceComponent: Rectangle {
            color: "#f9f9f9"
            opacity: 0.95

            Column {
                anchors.centerIn: parent
                spacing: 5

                Text {
                    width: 200
                    font.pointSize: main.fontPointSize + 7
                    font.weight: Font.ExtraLight
                    font.family: "Helvetica"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Add a person")
                }

                Text {
                    width: 200
                    color: "grey"
                    font.pointSize: main.fontPointSize - 1
                    font.weight: Font.ExtraLight
                    font.family: "Helvetica"
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Select a friend to add to Conversation %1").arg(chats.currentChatDialog.displayName)
                }
            }

            Image {
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.right: parent.right
                anchors.rightMargin: 25
                source: "images/close_popup.png"
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons
                preventStealing: true
                onClicked: {
                    mouse.accepted = false
                    chats.isSelectUser = false
                }

                onWheel: {
                }
            }
        }
    }

    Connections {
        target: chats
        onMuteUserChanged: {
            if (id === groupChatInfo.chatId) {
                toggleBtn.on = !isMute
            }
        }
    }

    onChatIdChanged: {
        if (dialog.isGroupChat) {
            dialog.groupChatHandler.getAllFields()
        }

        toggleBtn.on = !chats.isMuteUser(chatId)
    }
}
