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
    id: dialogsDelegate

    property color unreadHighlightColor: "#DFE3EA"

    width: dialogsDelegate.ListView.view.width
    height: avatar.height + 13

    HighlightListView {
        anchors.fill: parent
        visible: currentChatId === model.id
    }

    Rectangle {
        id: unreadHighlightFull
        anchors.fill: parent
        color: unreadHighlightColor
        opacity: 0.9
        visible: model.isUnread && !model.isOut
    }

    Row {
        id: dialog
        height: parent.height
        x: 10
        spacing: 7

        Avatar {
            id: avatar
            width: 40
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -1
            source: model.decoration
            online: model.online
        }

        Column {
            width: dialogsDelegate.width - dialog.x - avatar.width - dialog.spacing
            anchors.top: avatar.top
            anchors.topMargin: 1
            spacing: model.emoji && !model.typing && !selfAvatar.visible ? 4 : -1

            Item {
                width: parent.width
                height: name.height

                Text {
                    id: name
                    anchors.left: parent.left
                    anchors.right: messageDate.left
                    anchors.rightMargin: 5
                    color: "black"
                    font.pointSize: main.fontPointSize
                    font.bold: true
                    font.family: "Helvetica"
                    elide: Text.ElideRight
                    text: model.display
                }

                Text {
                    id: messageDate
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: name.verticalCenter
                    color: "#1769ad"
                    font.family: "Helvetica"
                    font.pointSize: main.fontPointSize - 2
                    text: model.dateStr
                }
            }

            Item {
                width: parent.width
                height: Math.max(textBody.height, selfAvatar.height + 7)

                AvatarImage {
                    id: selfAvatar
                    width: avatar.height * 0.55
                    height: visible ? avatar.height * 0.55 : 0
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    visible: (model.isOut || model.isGroupChat) && (!model.typing)
                    source: engine.getUser(model.from).photoMediumRect
                    radius: 15
                }

                TextEditItem {
                    id: textBody
                    anchors.left: selfAvatar.visible ? selfAvatar.right : parent.left
                    anchors.leftMargin: selfAvatar.visible ? 5 : 0
                    anchors.right: unreadCount.visible ? unreadCount.left : parent.right
                    anchors.rightMargin: 7
                    anchors.verticalCenter: selfAvatar.verticalCenter
                    anchors.verticalCenterOffset: selfAvatar.visible ? -2 : 0
                    maximumLineCount: model.emoji || selfAvatar.visible ? 1 : 2
                    lineHeight: 0.9
                    color: "#707070"
                    font.pointSize: main.fontPointSize - 1
                    font.italic: model.typing
                    wrapMode: Text.Wrap
                    richText: model.typing ? dialogsHandler.typingText : model.body
                    activeFocusOnPress: false
                }

                Rectangle {
                    id: unreadHighlight
                    z: -1
                    anchors.fill: textBody
                    anchors.topMargin: -2
                    anchors.leftMargin: -2
                    anchors.bottomMargin: -5
                    anchors.rightMargin: -2
                    color: unreadHighlightColor
                    opacity: 0.9
                    radius: 3
                    visible: model.isUnread && selfAvatar.visible
                }

                BadgeItem {
                    id: unreadCount
                    anchors.right: parent.right
                    anchors.rightMargin: 7
                    anchors.top: parent.top
                    anchors.topMargin: model.emoji && !model.typing && !selfAvatar.visible ? -1 : 4
                    count: model.unreadCount
                }
            }
        }
    }

    SeparatorItem {
        id: separator
        anchors.top: dialogsDelegate.bottom
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            main.dialogCurrentIndexChanged(index);
        }
    }
}
