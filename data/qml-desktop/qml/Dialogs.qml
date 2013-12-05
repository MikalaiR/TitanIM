import QtQuick 2.1

Item {
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    ListView{
        id: dialogsView;
        anchors.fill: parent
        model: dialogsModel;
        highlight: HighlightListView { }
        delegate: DialogsDelegate { }
        spacing: 2
    }
}
