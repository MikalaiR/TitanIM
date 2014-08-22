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

import QtQuick 2.1

Loader {
    id: tab
    anchors.fill: parent

    property string title
    property string icon
    property int badge
    signal clicked

    property bool __inserted: false

    Accessible.role: Accessible.LayeredPane
    visible: false
}
