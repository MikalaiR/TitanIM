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

BorderImage {
    id: badgeItem

    property int count: 0

    source: "images/badge.png"
    visible: badgeItem.count > 0
    smooth: true
    width: Math.max(badgeItem.sourceSize.width, badgeLabel.implicitWidth + 12)
    height: Math.max(badgeItem.sourceSize.height, badgeLabel.implicitHeight)
    border { left: 7; top: 8; right: 8; bottom: 9 }

    Text {
        id: badgeLabel
        font.pixelSize: 10
        anchors.centerIn: parent
        color: "white"
        font.bold: true
        text: badgeItem.count
    }
}
