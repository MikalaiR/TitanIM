import QtQuick 2.0

Rectangle {
    id: onlineIcon
    radius: 10
    color: "#f9f9f9"

    Rectangle {
        width: parent.width - 4
        height: parent.height - 4
        radius: 10
        anchors.centerIn: parent
        color: "#80b5af"
    }
}
