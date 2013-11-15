import QtQuick 2.0

Item {
    id: rosterDelegate;
    width: rosterDelegate.ListView.view.width
    height: avatarBorder.height + 13

    Row {
        id: contact;
        height: parent.height;
        x: 10;
        spacing: 7;

        Rectangle {
            id: avatarBorder;
            width: (avatar.width + 2);
            height: (avatar.height + 2);
            anchors.verticalCenter: parent.verticalCenter;
            scale: 0.0;
            color: "black";

            Behavior on scale {
                NumberAnimation {
                    easing.type: Easing.InOutQuad;
                }
            }

            Image {
                id: avatar;
                width: 28;
                height: 28;
                anchors.centerIn: parent;
                smooth: true;
                source: model.decoration;
            }
        }

        Column {
            width: rosterDelegate.width - contact.x - avatarBorder.width - contact.spacing - 15
            anchors.verticalCenter: avatarBorder.verticalCenter
            anchors.verticalCenterOffset: -1
            spacing: -2

            Item {
                width: parent.width
                height: name.height

                Text {
                    id: name
                    width: Math.min(implicitWidth, parent.width - onlineImg.width)
                    anchors.left: parent.left
                    color: "black"
                    font.pointSize: 13
                    font.bold: true
                    font.family: "Helvetica"
                    elide: Text.ElideRight
                    text: model.display
                }

                Image {
                    id: onlineImg
                    width: visible ? implicitWidth : 0
                    anchors.left: name.right
                    anchors.leftMargin: 6
                    anchors.verticalCenter: name.verticalCenter
                    visible: model.online;
                    source: "images/ic_online_up.png";
                }
            }

            Text {
                id: activity
                width: parent.width
                height: text.length ? implicitHeight : 0
                maximumLineCount: 1
                lineHeight: 0.8
                elide: Text.ElideRight
                color: "#707070"
                font.pointSize: 13 - 1
                wrapMode: Text.Wrap
                text: model.activity
            }
        }
    }

    Column {
        id: separator
        width: parent.width
        height: rosterView.spacing
        anchors.top: rosterDelegate.bottom

        Rectangle {
            width: parent.width
            height: 1
            color: "#dbd7d4"
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#fcfcfc"
        }
    }

    MouseArea{
        anchors.fill: parent;
        onClicked: {
            rosterView.currentIndex = index
//            profilePage.clear();
//            titanim.slotShowProfile(model.uid);
//            titanim.slotWallGet(model.uid);
//            profilePage.uid = model.uid;
//            mainView.pageStack.push(profilePage);
        }
    }

    states:[
        State{
            name: "Show";
            when: avatar.status == Image.Ready;
            PropertyChanges {
                target: avatarBorder;
                scale: 1;
            }
        }
    ]
}
