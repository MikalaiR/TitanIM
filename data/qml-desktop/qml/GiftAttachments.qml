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
    id: giftAttachments

    implicitWidth: img.width
    implicitHeight: img.height + (labelRow.visible ? 35 : 0)

    Image {
        id: img
        width: Math.min(maximumWidth, 256)
        height: Math.min(maximumWidth, 256)
        smooth: true
        fillMode: Image.PreserveAspectFit
        source: items[0].srcBig
    }

    Row {
        id: labelRow
        anchors.top: img.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        visible: !isAttachInFwdMsg
        spacing: 5

        Image {
            source: "images/gift.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -1
        }

        Text {
            id: labelGift
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            color: "#7F7662"
            font.pointSize: main.fontPointSize
            text: qsTr("Gift")
        }
    }
}
