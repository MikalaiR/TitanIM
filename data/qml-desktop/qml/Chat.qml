import QtQuick 2.0

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

        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: "#9E9E9E"
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
    }

    Item {
        id: footer
        z: 1
        width: parent.width
        height: 38
        anchors.bottom: parent.bottom

        Rectangle {
            width: parent.width
            height: 1
            anchors.top: parent.top
            color: "#9E9E9E"
        }
    }
}
