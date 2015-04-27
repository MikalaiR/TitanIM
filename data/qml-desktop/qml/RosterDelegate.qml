/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

import QtQuick 2.0

Item {
    id: rosterDelegate;
    width: rosterDelegate.ListView.view.width
    height: avatar.height + 13

    MouseArea{
        anchors.fill: parent;
        onClicked: {
            main.rosterCurrentIndexChanged(index);
//            profilePage.clear();
//            titanim.slotShowProfile(model.uid);
//            titanim.slotWallGet(model.uid);
//            profilePage.uid = model.uid;
//            mainView.pageStack.push(profilePage);
        }
    }

    HighlightListView {
        anchors.fill: parent
        visible: currentChatId === model.uid
    }

    Row {
        id: contact;
        height: parent.height;
        x: 10;
        spacing: 7;

        AvatarImage {
            id: avatar
            width: 35
            height: 35
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -1
            source: model.decoration
            online: model.online
        }

        Column {
            width: rosterDelegate.width - contact.x - avatar.width - contact.spacing - 15
            anchors.verticalCenter: avatar.verticalCenter

            spacing: -2

            Text {
                id: name
                width: parent.width
                color: "black"
                font.pointSize: main.fontPointSize
                font.bold: true
                font.family: "Helvetica"
                elide: Text.ElideRight
                text: model.display
            }

            Text {
                id: activity
                width: parent.width
                height: text.length ? implicitHeight : 0
                maximumLineCount: 1
                lineHeight: 0.8
                elide: Text.ElideRight
                color: "#707070"
                font.pointSize: main.fontPointSize - 1
                wrapMode: Text.Wrap
                text: model.activity
            }
        }
    }

    SeparatorItem {
        id: separator
        anchors.top: rosterDelegate.bottom
    }
}
