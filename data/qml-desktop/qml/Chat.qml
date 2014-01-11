import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {

    Rectangle {
        anchors.fill: parent
        color: "#DFE3EA"
    }

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

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: avatar.right
            anchors.leftMargin: 10
            text: chats.currentChatDialog.displayName
        }
    }

    ListView {
        id: chatView;
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: footer.top
        model: chats.currentChatModel;
        delegate: ChatDelegate { }
        verticalLayoutDirection: ListView.BottomToTop
        clip: true

        onContentYChanged: {
            footerSeparator.opacity = (-chatView.contentY - chatView.height) / 20
        }
    }

    Item {
        id: footer
        z: 1
        width: parent.width
        height: 45
        anchors.bottom: parent.bottom

        HeaderSeparator {
            id: footerSeparator
            anchors.top: parent.top
        }

        TextField {
            property Component textfieldStyle: TextFieldStyle {
                background: Item {
                    implicitWidth: 100
                    implicitHeight: 26
                    BorderImage {
                        border.bottom: 14
                        border.top: 13
                        border.right: 15
                        border.left: 15
                        anchors.fill: parent
                        source: "images/input_field.png"
                    }
                }

                padding { top: 6 ; left: 12 ; right: 12; bottom:4 }
            }

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
            placeholderText: "Введите сообщение..."
            style: textfieldStyle
            font.family: "Helvetica"
            font.pixelSize: 12

            onAccepted: {
                chats.currentChat.sendMessage(text)
                text = ""
            }
        }
    }
}
