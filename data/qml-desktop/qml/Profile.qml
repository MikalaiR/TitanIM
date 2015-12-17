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
    property int uid: chats.currentChatId
    property var profileItem: engine.getUser(uid)
    property bool availablePhotosProfile: false

    function __friendStatusToText(status) {
        switch (status) {
        case 0:
            return qsTr("Add to friends", "request")
        case 1:
            return qsTr("Unfollow")
        case 2:
            return qsTr("Add to friends", "confirm")
        case 3:
            return qsTr("Remove from friends")
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    Flickable {
        id: flickableItem
        anchors.fill: parent
        anchors.topMargin: 25
        contentWidth: content.width
        contentHeight: content.height
        visible: uid !== 0

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
                        if (availablePhotosProfile) {
                            PhotoViewer.setItems(engine.photosProfile(uid), 0)
                        }
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
                anchors.topMargin: 35
                spacing: 6

                UnderlineLabel {
                    visible: text.length
                    label: qsTr("city")
                    text: profileItem.city
                    select: true
                }

                UnderlineLabel {
                    visible: text.length
                    label: qsTr("birthday")
                    text: profileItem.bdate
                    select: true
                }

                UnderlineLabel {
                    visible: text.length
                    label: qsTr("mobile")
                    text: profileItem.mobilePhone
                    select: true
                }

                UnderlineLabel {
                    visible: profileItem.domain.length
                    label: qsTr("link")
                    text: "https://vk.com/" + profileItem.domain

                    onClicked: {
                        Qt.openUrlExternally(text)
                    }
                }
            }

            Column {
                id: globalColumn2
                anchors.left: avatar.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: globalColumn1.bottom
                anchors.topMargin: 15
                spacing: 6

                UnderlineButton {
                    label: qsTr("Send message")

                    onClicked: {
                        mainWindow.popPage({item: Qt.resolvedUrl("Chat.qml")})
                    }
                }

                UnderlineButton {
                    label: __friendStatusToText(profileItem.friendStatus)

                    onClicked: {
                        if (profileItem.friendStatus === 0 || profileItem.friendStatus === 2) {
                            engine.addFriend(uid)
                        } else {
                            engine.deleteFriend(uid)
                        }
                    }
                }
            }

            Column {
                id: globalColumn3
                anchors.left: avatar.left
                anchors.right: parent.right
                anchors.rightMargin: 80
                anchors.top: globalColumn2.bottom
                anchors.topMargin: 15
                spacing: 6

                UnderlineToggleButton {
                    id: toggleBtn
                    label: qsTr("Notifications")
                    on: !chats.isMuteUser(uid)

                    onClickedToggle: {
                        chats.setMuteUser(uid, !toggleBtn.on)
                    }
                }

                UnderlineButton {
                    label: profileItem.blacklistedByMe ? qsTr("Unblock contact") : qsTr("Block contact")
                    color: profileItem.blacklistedByMe ? "#2B83DC" : "red"

                    onClicked: {
                        if (profileItem.blacklistedByMe) {
                            engine.unbanUser(uid)
                        } else {
                            engine.banUser(uid)
                        }
                    }
                }
            }


        }
    }

    Connections {
        target: engine
        onPhotosProfileLoaded: {
            if (uid === profile.uid) {
                profile.availablePhotosProfile = true
            }
        }
    }

    Connections {
        target: chats
        onMuteUserChanged: {
            if (id === profile.uid) {
                toggleBtn.on = !isMute
            }
        }
    }

    onUidChanged: {
        profile.availablePhotosProfile = false
        engine.getPhotosProfile(uid)
        engine.updateUser(uid)
        toggleBtn.on = !chats.isMuteUser(uid)
    }
}

