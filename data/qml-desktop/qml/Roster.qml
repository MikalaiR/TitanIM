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

Item {
    anchors.fill: parent

    property int currentChatId: chats.currentChatId

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    ListView {
        id: rosterView;
        anchors.fill: parent
        model: rosterHandler.proxy;
        delegate: RosterDelegate { }
        spacing: 2
        section.property: "alphabet"
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: SectionHeading { }
    }

    onCurrentChatIdChanged: {
        if (tabBar.currentIndex === 1)
            return;

        var index = rosterHandler.indexOf(currentChatId)
        if (index === -1) index = 0
        rosterView.positionViewAtIndex(index, ListView.Center)
    }
}
