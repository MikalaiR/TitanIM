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
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {
    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        TabBarView {
            id: tabBar
            width: 280
            height: parent.height
            Layout.minimumWidth: 200
            Layout.maximumWidth: 500

            TabBarItem {
                title: "1"
                icon: "images/messages_tab.png"
                Dialogs { }
            }

            TabBarItem {
                title: "2"
                icon: "images/contacts_tab.png"
                Roster { }
            }

            TabBarItem {
                title: "3"
                icon: "images/favorites_tab.png"
                Text{text: "3"}
            }

            TabBarItem {
                title: "4"
                icon: "images/settings_tab.png"
                Text{text: "4"}
            }
        }

        Chat {
            Layout.fillWidth: true
        }
    }
}
