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
    id: avatarImage

    property string source: ""
    property int border: 2

    scale: 0.0
    color: "#868a8f"

    Behavior on scale {
        NumberAnimation {
            easing.type: Easing.InOutQuad
        }
    }

    Image {
        id: img
        width: parent.width - avatarImage.border
        height: parent.height - avatarImage.border
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        smooth: true
        source: avatarImage.source
    }

    states: [
        State {
            name: "Show"
            when: img.status == Image.Ready
            PropertyChanges {
                target: avatarImage
                scale: 1
            }
        }
    ]
}
