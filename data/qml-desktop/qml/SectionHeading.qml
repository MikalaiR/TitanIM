import QtQuick 2.0

Item {
    id: sectionHeading
    width: parent.width
    height: 17

    Rectangle {
        anchors.fill: parent
        color: "#a4a3a3"

        Text {
            x: 15
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
            color: "white"
            font.pixelSize: 13
            font.bold: true
            font.family: "Helvetica"
            text: section
        }
    }
}
