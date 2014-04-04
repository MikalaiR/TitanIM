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
    id: onlineIcon
    radius: 10
    color: "#f9f9f9"

    Rectangle {
        width: parent.width - 4
        height: parent.height - 4
        radius: 10
        anchors.centerIn: parent
        color: "#80b5af"
    }
}
