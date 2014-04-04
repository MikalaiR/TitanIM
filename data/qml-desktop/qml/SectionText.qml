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
    id: sectionText

    property string text: ""

    height: visible ? textShadow.height + 10 : 0
    width: textShadow.width

    TextShadow {
        id: textShadow
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 8
        font.pointSize: 13 - 2
        font.bold: true
        font.family: "Helvetica"
        color: "#8B92A4"
        text: sectionText.text
    }
}
