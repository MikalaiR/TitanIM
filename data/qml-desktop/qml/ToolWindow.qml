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
import QtQuick.Window 2.2

Window {
    id: toolWindow

    property int width2: 0
    property int height2: 0

    signal clicked

    onWidthChanged: {
        var temp = (x + (width2 / 2)) - (width / 2)
        width2 = width

        window.x = temp
    }

    onHeightChanged: {
        var temp = (y + (height2 / 2)) - (height / 2)
        height2 = height

        window.y = temp
    }

    MouseArea {
        anchors.fill: parent
        property int lx
        property int ly
        property bool isMove: false

        onPressed: {
            lx = mouse.x
            ly = mouse.y

            isMove = false
        }

        onClicked: {
            if (!isMove) {
                toolWindow.clicked()
            }
        }

        onPositionChanged: {
            var p = main.positionGlobalCursor()
            window.setX(p.x - lx)
            window.setY(p.y - ly)

            isMove = true
        }
    }

    Component.onCompleted: {
        var p = main.positionGlobalCenter(toolWindow.width, toolWindow.height)
        toolWindow.setX(p.x)
        toolWindow.setY(p.y)
        visible: true
    }
}
