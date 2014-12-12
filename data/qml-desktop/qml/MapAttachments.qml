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
    id: mapAttachments

    implicitWidth: img.width
    implicitHeight: img.height

    Image {
        id: img
        width: maximumWidth < items[0].width ? maximumWidth : items[0].width
        height: width * items[0].height / items[0].width
        smooth: true
        fillMode: Image.Tile
        source: items[0].src

        Image {
            id:mapPin
            anchors.centerIn: parent
            source: "images/map_pin.png"
            visible: img.status === Image.Ready
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally(items[0].url);
        }
    }
}
