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
import QtQuick.Dialogs 1.0

Item {
    id:chatFrame

    property color backgroundColor: "#DFE3EA"
    property color unreadHighlightColor: "#D6DDE8"

    Rectangle {
        id: backgroundFrame
        anchors.fill: parent
        color: backgroundColor
    }

    Item {
        id: contentFrame
        anchors.fill: parent
        visible: chats.currentChatId !== 0

        Item {
            id: header
            z: 1
            width: parent.width
            height: 38
            anchors.top: parent.top

            HeaderSeparator {
                anchors.top: parent.bottom
                anchors.topMargin: -1
            }

            AvatarItem {
                id: avatar
                width: 28
                height: 28
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                source: chats.currentChatDialog.decoration
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: avatar.right
                anchors.leftMargin: 10
                spacing: -1

                Text {
                    id: name
                    text: chats.currentChatDialog.displayName
                }

                TextShadow {
                    font.pointSize: 13 - 2
                    color: "gray"
                    text: chats.currentChatDialog.description
                }
            }

            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                source: "images/actions.png"
            }
        }

        ListView {
            id: chatView
            width: parent.width
            anchors.top: header.bottom
            anchors.bottom: footer.top
            model: chats.currentChatModel
            delegate: ChatDelegate { }
            verticalLayoutDirection: ListView.BottomToTop
            clip: true
            section.property: "section"

            onContentYChanged: {
                footerSeparator.opacity = (-chatView.contentY - chatView.height) / 20
            }

            Connections {
                target: chats
                onCurrentChatIdChanged: {
                    chatView.positionViewAtIndex(0, ListView.Beginning);
                }
            }
        }

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            nameFilters: ["Image files (*.jpg *.jpeg *.png *.bmp *.gif)"]
            selectMultiple: true

            onAccepted: {
                chats.currentChat.addAttachments(fileDialog.fileUrls)
            }
        }

        Item {
            id: footer
            z: 1
            width: parent.width
            height: textArea.height + 19
            anchors.bottom: parent.bottom

            HeaderSeparator {
                id: footerSeparator
                anchors.top: parent.top
            }

            Image {
                id: uploadBtn
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: "images/attach.png"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        fileDialog.open()
                    }
                }
            }

            TextAreaItem {
                id: textArea
                height: 26
                anchors.left: uploadBtn.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 1
                placeholderText: qsTr("Write a message...")
                font.family: "Helvetica"
                font.pixelSize: 12

                onAccepted: {
                    chats.currentChat.sendMessage(text)
                    text = ""
                }

                onTextChanged: {
                    if (!timerSendTyping.running) {
                        chats.currentChat.sendTyping()
                        timerSendTyping.start()
                    }
                }

                Timer {
                    id: timerSendTyping
                    interval: 5000
                }
            }
        }
    }

    Item {
        id: labelFrame
        anchors.fill: parent
        visible: chats.currentChatId === 0

        TextShadow {
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 18
            font.bold: true
            font.family: "Helvetica"
            color: "#8B92A4"
            text: qsTr("Select the dialog to start a conversation")
            wrapMode: Text.Wrap
        }
    }
}
