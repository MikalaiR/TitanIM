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

import QtQuick 2.2

Item {
    id: spinnerItem
    implicitWidth: img.width
    implicitHeight: img.height

    property bool running: false
    property double progress: -1
    property color color: "black"

    Image {
        id: img
        width: 74
        height: 74
        anchors.centerIn: parent
        visible: running
        source: "images/spinner.png"

        RotationAnimator on rotation {
            running: running
            loops: Animation.Infinite
            duration: 750
            from: 0 ; to: 360
        }
    }

    Text {
        id: label
        anchors.centerIn: img
        color: spinnerItem.color
        font.pointSize: 11
        text: Math.round(progress * 100) + "%"
        visible: running && (progress !== -1)
    }
}
