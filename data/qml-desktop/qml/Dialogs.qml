import QtQuick 2.1

Item {
    anchors.fill: parent

    property int currentChatId: chats.currentChatId

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    ListView{
        id: dialogsView;
        anchors.fill: parent
        model: dialogsModel;
        delegate: DialogsDelegate { }
        spacing: 2
    }
}
