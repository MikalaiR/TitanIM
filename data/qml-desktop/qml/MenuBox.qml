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
import QtQuick.Controls 1.4

Menu {
    id: menuBox
    property int width: 0
    property int height: 0

    function showMenu(x, y) {
        menuBox.__popup(Qt.rect(x, y, 0, 0))
    }

    Component.onCompleted: {
        var maxWidth = 0

        for (var i = 0; i < menuBox.items.length; i++) {
            var textItem = Qt.createQmlObject('import QtQuick 2.0;
                                               Text {
                                                   font.pointSize:' + menuBox.__font.pointSize + ';
                                                   text: "' + menuBox.items[i].text + '";
                                                   visible: false
                                               }', menuBox)
            maxWidth = Math.max(maxWidth, textItem.width + 5)

            menuBox.height = textItem.height * menuBox.items.length + 12
        }

        menuBox.width = maxWidth
    }
}
