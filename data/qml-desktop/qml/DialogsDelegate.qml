import QtQuick 2.0

Item {
    id: dialogsDelegate
    width: dialogsDelegate.ListView.view.width
    height: avatar.height + 13

    MouseArea {
        anchors.fill: parent
        onClicked: {
            main.dialogCurrentIndexChanged(index);
        }
    }

    HighlightListView {
        anchors.fill: parent
        visible: currentChatId === model.id
    }

    Row {
        id: dialog
        height: parent.height
        x: 10
        spacing: 7

        AvatarItem {
            id: avatar
            width: 40
            height: 40
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -1
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
            width: dialogsDelegate.width - dialog.x - avatar.width - dialog.spacing
            anchors.top: avatar.top
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
