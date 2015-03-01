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
    id: fwdMsgAttachments

    property var messages: items[0].messages
    property int maxWidth: maximumWidth

    implicitWidth: messagesColumn.width
    implicitHeight: messagesColumn.height

    Column {
        id: messagesColumn

        Repeater {
            model: fwdMsgAttachments.messages

            Item {
                id: messageBlock

                property var profile: engine.getUser(modelData.uid)

                width: Math.max(body.contentWidth, attachmentsView.width, name.contentWidth + 37) + 7
                height: header.height + (body.contentHeight + attachmentsView.height) + (body.height ? 10 : -10)

                Row {
                    anchors.fill: parent
                    spacing: 5

                    Rectangle {
                        id: line
                        width: 2
                        height: message.height + 5
                        color: "grey"
                    }

                    Column {
                        id: message
                        spacing: 3

                        Row {
                            id: header
                            width: parent.width
                            height: childrenRect.height
                            spacing: 5

                            AvatarImage {
                                id: avatar
                                width: 30
                                height: 30
                                source: profile.photoMediumRect
                            }

                            Column {
                                width: fwdMsgAttachments.maxWidth - avatar.width - 14
                                height: childrenRect.height
                                anchors.verticalCenter: avatar.verticalCenter
                                spacing: 3

                                Text {
                                    id: name
                                    anchors.left: parent.left
                                    width: parent.width
                                    color: "black"
                                    font.pointSize: 13
                                    font.bold: true
                                    font.family: "Helvetica"
                                    elide: Text.ElideRight
                                    text: profile.fullName
                                }

                                Text {
                                    id: messageDate
                                    anchors.left: parent.left
                                    width: parent.width
                                    color: "#1769ad"
                                    font.family: "Helvetica"
                                    font.pointSize: 13 - 2
                                    elide: Text.ElideRight
                                    text: Qt.formatDateTime(modelData.date, "dd.MM.yyyy, hh:mm")
                                }
                            }
                        }

                        Column {
                            id: content
                            spacing: 4

                            TextEditItem {
                                id: body
                                width: fwdMsgAttachments.maxWidth - 14
                                height: text.length ? implicitHeight + 2: 0
                                verticalAlignment: Text.AlignTop
                                font.pointSize: 13 - 1
                                richText: modelData.body
                                wrapMode: Text.Wrap
                                readOnly: true
                                selectByMouse: true
                            }

                            Loader {
                                id: attachmentsView
                                anchors.left: body.left
                                width: Math.min(fwdMsgAttachments.maxWidth - 7, implicitWidth)
                                active: modelData.attachments && modelData.attachments.count()

                                sourceComponent: AttachmentsView {
                                    maximumWidth: fwdMsgAttachments.maxWidth - 7
                                    attachments: modelData.attachments
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
