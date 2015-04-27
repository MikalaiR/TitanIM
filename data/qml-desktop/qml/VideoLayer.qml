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
    id: videoLayer

    Image {
        id: icon
        width: Math.min(parent.width * 0.33, 50)
        height: Math.min(parent.height * 0.33, 50)
        anchors.centerIn: parent
        smooth: true
        source: "images/video_play.png"
    }

    Rectangle {
        width: text.width + 2
        height: text.height
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        color: "black"
        visible: video.duration

        Text {
            id: text
            anchors.centerIn: parent
            font.pointSize: main.fontPointSize - 2
            color: "white"
            text: video.durationStr
        }
    }
}
