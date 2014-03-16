import QtQuick 2.0

Text {
    id: textShadow

    Text {
        z: textShadow.z - 1
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 1
        text: textShadow.text
        color: "white"
        font: textShadow.font
    }
}
