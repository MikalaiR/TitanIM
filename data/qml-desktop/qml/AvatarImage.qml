import QtQuick 2.0

Rectangle {
    id: avatarImage

    property string source: ""
    property int border: 2

    scale: 0.0
    color: "#868a8f"

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.InOutQuad
        }
    }

    Image {
        id: img
        width: parent.width - avatarImage.border
        height: parent.height - avatarImage.border
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: avatarImage.source
    }

    states: [
        State {
            name: "Show"
            when: img.status == Image.Ready
            PropertyChanges {
                target: avatarImage
                scale: 1
            }
        }
    ]
}
