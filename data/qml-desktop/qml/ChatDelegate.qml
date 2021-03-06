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
import QtQuick.Controls 1.4
import TitanIM 2.0

Item {
    id: chatDelegate
    width: chatDelegate.ListView.view.width
    height: Math.max(avatar.height + 4, bubble.height + 9, service.height + 15, typing.height) + sectionText.height

    Rectangle {
        id: unreadHighlight
        anchors.top: content.top
        anchors.bottom: content.bottom
        anchors.left: chatDelegate.left
        anchors.right: chatDelegate.right
        color: chatFrame.unreadHighlightColor
        visible: model.messageType === MessageBase.Text && model.isUnread && !service.active
    }

    Loader {
        id: sectionText
        anchors.horizontalCenter: parent.horizontalCenter
        active: model.messageType !== MessageBase.Typing

        sourceComponent: SectionText {
            visible: chatDelegate.ListView.nextSection !== chatDelegate.ListView.section
            text: model.section
        }
    }

    Item {
        id: content
        property var profile: model.isOut ? engine.getUser() : engine.getUser(model.uid)
        anchors.top: sectionText.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Loader {
            id: checkBox
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 2
            active: chatFrame.isEdit && !service.active

            sourceComponent: Image {
                source: model.isChecked ? "images/message_checked.png" : "images/message_unchecked.png"
            }
        }

        Loader {
            id: avatar
            width: active ? item.width : 0
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            LayoutMirroring.enabled: model.messageType === MessageBase.Text && model.isOut
            active: !service.active && !chatFrame.isEdit

            sourceComponent: AvatarImage {
                width: 32
                height: 32
                source: content.profile.photoMediumRect

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        main.openChat(content.profile.id)
                    }
                }
            }
        }

        Loader {
            id: typing
            anchors.left: avatar.right
            anchors.leftMargin: 4
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: 2
            active: model.messageType === MessageBase.Typing

            sourceComponent: Image {
                height: 24;
                fillMode: Image.PreserveAspectFit
                source: "images/typing.png"
            }
        }

        Loader {
            id: service
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            active: model.messageType === MessageBase.Service || (model.messageType === MessageBase.Text && model.action !== -1)

            sourceComponent: Column {
                spacing: 4

                TextEditItem {
                    id: body
                    width: chatDelegate.width * 0.9
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: main.fontPointSize - 2
                    wrapMode: Text.Wrap
                    readOnly: true
                    selectByMouse: true
                    color: "#8B92A4"
                    text: model.messageType === MessageBase.Service
                          ? model.display
                          : chats.currentChat.actionToString(content.profile.fullName, model.action, model.actionText, content.profile.sex);
                }

                Loader {
                    id: attachmentsView
                    anchors.horizontalCenter: parent.horizontalCenter
                    active: model.attachments && model.attachments.count()

                    sourceComponent: AttachmentsView {
                        maximumWidth: chatDelegate.width * 0.9
                        attachments: model.attachments
                    }
                }
            }
        }

        Loader {
            id: bubble
            anchors.left: avatar.right
            anchors.leftMargin: checkBox.active && !model.isOut ? 35 : 3
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: 2
            LayoutMirroring.enabled: model.isOut
            active: model.messageType === MessageBase.Text && !service.active

            sourceComponent: BubbleItem {
                maximumWidth: chatDelegate.width * 0.6
                color: model.isGift ? "#F4EBD7" : "transparent"
                modelData: model
            }
        }
    }

    Loader {
        id: contextMenu
        active: false

        sourceComponent: Menu {
            id: ctxMenu

            MenuItem {
                text: qsTr("Copy text")
                onTriggered: {
                    model.isChecked = true
                    chats.currentChat.copyTextSelectedItems()
                    chatFrame.isEdit = false
                    contextMenu.active = false
                }
            }

            MenuItem {
                text: qsTr("Forward message")
                onTriggered: {
                    model.isChecked = true
                    chats.isForward = true
                    contextMenu.active = false
                }
            }

            MenuItem {
                text: qsTr("Delete message")
                onTriggered: {
                    model.isChecked = true
                    chats.currentChat.deleteSelectedItems()
                    chatFrame.isEdit = false
                    contextMenu.active = false
                }
            }

            MenuItem {
                text: qsTr("Select message")
                onTriggered: {
                    model.isChecked = true
                    chatFrame.isEdit = true
                    contextMenu.active = false
                }
            }
        }

        onActiveChanged: {
            if (active) {
                item.popup()
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: checkBox.active ? (Qt.LeftButton | Qt.RightButton) : Qt.RightButton
        onClicked: {
            if (checkBox.active) {
                model.isChecked = mouse.button === Qt.RightButton ? true : !model.isChecked
            }

            if (mouse.button === Qt.RightButton) {
                contextMenu.active = false
                contextMenu.active = true
            }
        }

        onEntered: {
            if (model.isUnread)
            {
                chats.currentChat.markAsRead()
            }
        }
    }
}
