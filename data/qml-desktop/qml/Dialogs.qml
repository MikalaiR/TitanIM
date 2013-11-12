import QtQuick 2.1

Item {
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"
    }

    ListView{
        id: dialogsView;
        anchors.fill: parent
        model: dialogsModel;
        highlight: Rectangle { color: "lightsteelblue" }
        delegate: DialogsDelegate { }
        spacing: 2
    }
}
