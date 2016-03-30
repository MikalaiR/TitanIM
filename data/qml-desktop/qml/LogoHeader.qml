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

Row {
    id: logoHeader
    spacing: 17

    Image {
        id: logo
        smooth: true
        source: "images/logo_vk.png"
        fillMode: Image.PreserveAspectFit
    }

    Column {
        id: appName
        anchors.verticalCenter: parent.verticalCenter
        spacing: 7

        Text {
            text: "TitanIM"
            font.pointSize: 21
        }

        Text {
            text: qsTr("Client for social network VK")
            font.pointSize: 11
            color: "grey"
        }
    }
}
