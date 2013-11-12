import QtQuick 2.1

Loader {
    id: tab
    anchors.fill: parent

    property string title
    property string icon

    property bool __inserted: false

    Accessible.role: Accessible.LayeredPane
    visible: false
}
