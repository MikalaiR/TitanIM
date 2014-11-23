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
    id: attachmentDelegate
    width: attachmentDelegate.ListView.view.height - 10
    height: width

    Image {
        anchors.fill: parent
        source: model.decoration
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            chats.currentChat.removeAttachment(index)
        }
    }
}
