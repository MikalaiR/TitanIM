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
import QtGraphicalEffects 1.0

Item {
    id: avatarImage

    property string source: ""
    property bool online: false
    property int radius: 30

    Image {
        id: img
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: avatarImage.source
        visible: false
    }

    Rectangle {
        id: mask
        anchors.fill: img
        radius: avatarImage.radius
        visible: false
    }

    OpacityMask {
        anchors.fill: img
        source: img
        maskSource: mask
    }

    OnlineIcon {
        id: onlineIcon
        width: 12
        height: 12
        anchors.right: parent.right
        anchors.rightMargin: -3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -3
        visible: avatarImage.online;
    }
}
