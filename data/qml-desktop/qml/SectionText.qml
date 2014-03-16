import QtQuick 2.0

Item {
    id: sectionText

    property string text: ""

    height: visible ? textShadow.height + 10 : 0
    width: textShadow.width

    TextShadow {
        id: textShadow
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 8
        font.pointSize: 13 - 2
        font.bold: true
        font.family: "Helvetica"
        color: "#8B92A4"
        text: sectionText.text
    }
}
