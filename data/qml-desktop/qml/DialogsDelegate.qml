import QtQuick 2.0

Item {
    id: dialogsDelegate
    width: dialogsDelegate.ListView.view.width
    height: avatarBorder.height + 13

    MouseArea {
        anchors.fill: parent
        onClicked: {
            dialogsView.currentIndex = index
//            titanim.slotOpenChatWindow(model.uid);
//            mainView.pageStack.push(chatPage);
        }
    }

    Row {
        id: dialog
        height: parent.height
        x: 10
        spacing: 7

        AvatarItem {
            id: avatarBorder
            width: 41
            height: 41
            anchors.verticalCenter: parent.verticalCenter
            source: model.decoration

            MouseArea {
                anchors.fill: parent
                onClicked: {
//                    if (model.uid > 0){
//                        profilePage.clear();
//                        titanim.slotShowProfile(model.uid);
//                        titanim.slotWallGet(model.uid);
//                        profilePage.uid = model.uid;
//                        mainView.pageStack.push(profilePage);
//                    }
                }
            }
        }

        Column {
            width: dialogsDelegate.width - dialog.x - avatarBorder.width - dialog.spacing
            anchors.top: avatarBorder.top
            anchors.topMargin: 1
            spacing: -2

            Item {
                width: parent.width
                height: name.height

                Text {
                    id: name
                    anchors.left: parent.left
                    anchors.right: messageDate.left
                    anchors.rightMargin: 5
                    color: "black"
                    font.pointSize: 13
                    font.bold: true
                    font.family: "Helvetica"
                    elide: Text.ElideRight
                    text: model.display
                }

                Text {
                    id: messageDate
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: name.verticalCenter
                    color: "#1769ad"
                    font.family: "Helvetica"
                    font.pointSize: 13 - 2
                    text: model.dateStr
                }
            }

            Text {
                id: textBody
                width: parent.width
                maximumLineCount: 2
                lineHeight: 0.8
                elide: Text.ElideRight
                color: "#707070"
                font.pointSize: 13 - 1
                wrapMode: Text.Wrap
                text: model.body
            }
        }
    }

    SeparatorItem {
        id: separator
        anchors.top: dialogsDelegate.bottom
    }
}
