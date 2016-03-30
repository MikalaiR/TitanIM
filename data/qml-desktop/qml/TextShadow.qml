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

Text {
    id: textShadow

    Text {
        z: textShadow.z - 1
        width: textShadow.width
        height: textShadow.height
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 1
        horizontalAlignment: textShadow.horizontalAlignment
        text: textShadow.text
        color: "white"
        font: textShadow.font
        lineHeight: textShadow.lineHeight
        wrapMode: textShadow.wrapMode
    }
}
