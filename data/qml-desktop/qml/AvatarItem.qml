import QtQuick 2.0

Rectangle {
    id: avatarItem

    property string source: ""
    property int border: 2

    scale: 0.0
    color: "black"

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.InOutQuad
        }
    }

    Image {
        id: avatar
        width: parent.width - avatarItem.border
        height: parent.height - avatarItem.border
        anchors.centerIn: parent
        smooth: true
        source: avatarItem.source
    }

    states: [
        State {
            name: "Show"
            when: avatar.status == Image.Ready
            PropertyChanges {
                target: avatarBorder
                scale: 1
            }
        }
    ]
}
