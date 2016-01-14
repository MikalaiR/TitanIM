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

Rectangle {
    id: badgeItem

    property int count: 0

    color: "#50A4E0"
    radius: 10
    visible: badgeItem.count > 0
    width: badgeLabel.implicitWidth + 14
    height: badgeLabel.implicitHeight + 7

    Text {
        id: badgeLabel
        font.pixelSize: 10
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 1
        color: "white"
        text: badgeItem.count
    }
}
