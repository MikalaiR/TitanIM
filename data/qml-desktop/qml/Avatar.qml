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
    id: avatar

    property var source
    property bool online: false

    Component {
        id: avatarItem

        AvatarItem {
            source: url
        }
    }

    Row {
        id: row
        width: parent.width
        height: parent.height
        spacing: 1
        visible: false

        Column {
            width: i2.active ? parent.width / 2 - 1 : parent.width
            height: parent.height
            spacing: 1

            AvatarItem {
                id: i1
                width: parent.width
                height: i3.active ? parent.height / 2 - 1 : parent.height
                source: avatar.source[0]
            }

            Loader {
                id: i3
                property string url: active ? avatar.source[2] : ""
                width: parent.width
                height: parent.height / 2 - 1
                active: avatar.source.length > 2
                sourceComponent: avatarItem
            }
        }

        Column {
            width: parent.width / 2 - 1
            height: parent.height
            spacing: 1

            Loader {
                id: i2
                property string url: active ? avatar.source[1] : ""
                width: parent.width
                height: i4.active ? parent.height / 2 - 1 : parent.height
                active: avatar.source.length > 1
                sourceComponent: avatarItem
            }

            Loader {
                id: i4
                property string url: active ? avatar.source[3] : ""
                width: parent.width
                height:  parent.height / 2 - 1
                active: avatar.source.length > 3
                sourceComponent: avatarItem
            }
        }
    }

    Rectangle {
        id: mask
        anchors.fill: row
        radius: 30
        visible: false
    }

    OpacityMask {
        anchors.fill: row
        source: row
        maskSource: mask
    }

    OnlineIcon {
        id: onlineIcon
        width: 12
        height: 12
        anchors.right: parent.right
        anchors.rightMargin: -2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -2
        visible: avatar.online;
    }
}
