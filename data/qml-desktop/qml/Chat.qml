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
import QtQuick.Dialogs 1.0

Item {
    id:chatFrame

    readonly property string name: "chat"
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

            Item {
                width: childrenRect.width
                height: parent.height

                Avatar {
                    id: avatar
                    width: 28
                    height: 28
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    source: chats.currentChatDialog.decoration
                }

                Item {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: avatar.right
                    anchors.leftMargin: 10
                    height: childrenRect.height
                    width: childrenRect.width

                    Column {
                        spacing: -1

                        Text {
                            id: name
                            text: chats.currentChatDialog.displayName
                        }

                        TextShadow {
                            id: desc
                            font.pointSize: main.fontPointSize - 2
                            color: "gray"
                            text: chats.currentChatDialog.description
                        }
                    }

                    Behavior on height {
                        enabled: desc.text && chatView.count > 0
                        NumberAnimation { duration: 80 }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!chats.currentChat.isGroupChat) { //todo
                            mainWindow.pushPage(Qt.resolvedUrl("Profile.qml"), "profile")
                        }
                    }
                }
            }

            Image {
                id: profileMenuBtn
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                source: "images/actions.png"

                MenuBox {
                    id: profileMenu

                    MenuItem {
                        text: qsTr("Open on VK")
                        onTriggered: {
                            Qt.openUrlExternally("https://vk.com/" + engine.getUser(chats.currentChatId).domain)
                        }
                    }

                    MenuItem {
                        text: qsTr("Clear message history")
                        onTriggered: {
                            chats.currentChat.clearHistory()
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var point = profileMenuBtn.mapToItem(null, 0, 0)
                        profileMenu.showMenu(point.x - profileMenu.width, point.y + profileMenuBtn.height)
                    }
                }
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
        }

        FileDialog {
            id: fileDialog
            property bool asDoc: false
            title: "Please choose a file"
            selectMultiple: true

            onAccepted: {
                chats.currentChat.addAttachments(fileDialog.fileUrls, asDoc)
            }
        }

        Item {
            id: footer
            z: 1
            width: parent.width
            height: inputArea.height + attachList.height + 19
            anchors.bottom: parent.bottom

            HeaderSeparator {
                id: footerSeparator
                anchors.top: parent.top
            }

            Row {
                id: inputArea
                height: textArea.height
                anchors.top: parent.top
                anchors.topMargin: 11
                anchors.left: parent.left
                anchors.leftMargin: 15
                anchors.right: parent.right
                anchors.rightMargin: 15
                spacing: 10

                Image {
                    id: uploadBtn
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/attach.png"

                    MenuBox {
                        id: uploadMenu

                        MenuItem {
                            text: qsTr("Photo")
                            onTriggered: {
                                fileDialog.nameFilters = ["Image files (*.jpg *.jpeg *.png *.bmp *.gif)", "All files (*)"]
                                fileDialog.asDoc = false
                                fileDialog.open()
                            }
                        }

                        MenuItem {
                            text: qsTr("Video")
                            onTriggered: {
                                fileDialog.nameFilters = ["Video files (*.avi *.mpg)", "All files (*)"]
                                fileDialog.asDoc = false
                                fileDialog.open()
                            }
                        }

                        MenuItem {
                            text: qsTr("Audio file")
                            onTriggered: {
                                fileDialog.nameFilters = ["Audio files (*.mp3)", "All files (*)"]
                                fileDialog.asDoc = false
                                fileDialog.open()
                            }
                        }

                        MenuItem {
                            text: qsTr("Document")
                            onTriggered: {
                                fileDialog.nameFilters = ["All files (*)"]
                                fileDialog.asDoc = true
                                fileDialog.open()
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        anchors.margins: -7
                        onClicked: {
                            var point = uploadBtn.mapToItem(null, 0, 0)
                            uploadMenu.showMenu(point.x - 5, point.y - uploadMenu.height)
                        }
                    }
                }

                TextAreaItem {
                    id: textArea
                    minimumHeight: 26
                    width: parent.width - uploadBtn.width - 5
                    placeholderText: qsTr("Write a message...")
                    font.family: "Helvetica"
                    font.pixelSize: 12
                    text: chats.currentChat.textMessage

                    onAccepted: {
                        chats.currentChat.sendMessage(textArea.getFormattedText())
                        chats.currentChat.textMessage = ""
                    }

                    onTextChanged: {
                        chats.currentChat.textMessage = text
                    }

                    onPressed: {
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

            Loader {
                id: attachList
                anchors.top: inputArea.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                height: active ? 80 : 0
                active: chats.currentChatAttachments.count

                sourceComponent: ListView {
                    property int oldCount: 0
                    spacing: 3
                    clip: true
                    orientation: ListView.Horizontal
                    model: chats.currentChatAttachments
                    delegate: AttachmentsDelegate { }
                    footer: Item { width: attachList.anchors.leftMargin }

                    remove: Transition {
                        ParallelAnimation {
                            NumberAnimation { property: "opacity"; to: 0; duration: 200 }
                            NumberAnimation { properties: "y"; to: 100; duration: 200 }
                        }
                    }

                    removeDisplaced: Transition {
                        NumberAnimation { properties: "x,y"; duration: 200 }
                    }

                    onCountChanged: {
                        if (oldCount !== count) {
                            if (count > oldCount) {
                                positionViewAtEnd()
                            }

                            oldCount = count
                        }
                    }
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
            font.pointSize: main.fontPointSize + 5
            font.bold: true
            font.family: "Helvetica"
            color: "#8B92A4"
            text: qsTr("Select the dialog to start a conversation")
            wrapMode: Text.Wrap
        }
    }

    Connections {
        target: chats
        onCurrentChatIdChanged: {
            chatView.positionViewAtBeginning()
            textArea.cursorPositionToEnd()
            textArea.forceActiveFocus()
        }
    }
}
