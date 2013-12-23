import QtQuick 2.0

Item {
    id: chatDelegate
    width: chatDelegate.ListView.view.width
    height: bubble.height + 9

    Row {
        id: msg
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        spacing: 3
        LayoutMirroring.enabled: model.isOut

        AvatarImage {
            id: avatar
            width: 32
            height: 32
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            source: model.decoration
            visible: model.decoration
        }

        BorderImage {
            id: bubble
            width: body.contentWidth + 32
            height: body.contentHeight + 11
            anchors.bottom: avatar.bottom
            anchors.bottomMargin: -2
            source: model.isOut ? "images/blue_bubble.sci" : "images/grey_bubble.sci"
            smooth: true

            Text {
                id: body
                width: Math.min(body.implicitWidth, chatDelegate.width * 0.6)
                anchors.left: bubble.left
                anchors.leftMargin: model.isOut ? 12 : 17
                anchors.top: bubble.top
                anchors.topMargin: 3
                font.pointSize: 13 - 1
                lineHeight: 0.9
                wrapMode: Text.Wrap
                text: model.body
            }
        }
    }
}
