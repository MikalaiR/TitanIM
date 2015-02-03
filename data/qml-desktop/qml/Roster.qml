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
    property bool filtered: textArea.length

    function scrollToTop() {
        rosterView.positionViewAtBeginning()
    }

    function clearFilter() {
        textArea.text = ""
    }

    function forceActiveFocusFilter() {
        textArea.forceActiveFocus()
    }

    Rectangle {
        anchors.fill: parent
        color: "#f9f9f9"
    }

    Item {
        id: header
        height: 38
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        TextAreaItem {
            id: textArea
            minimumHeight: 24
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Helvetica"
            font.pixelSize: 12
            icon: "images/search.png"
            placeholderText: qsTr("Search...")
            smile: false

            onTextChanged: {
                rosterHandler.setFilterWildcard(getText())
            }
        }
    }

    ListView {
        id: rosterView;
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip: true
        model: rosterHandler.proxy;
        delegate: RosterDelegate { }
        spacing: 2
        section.property: "alphabet"
        section.labelPositioning: ViewSection.InlineLabels
        section.delegate: SectionHeading { }
    }

    onCurrentChatIdChanged: {
        if (tabBar.currentIndex === 1 && !filtered)
            return;

        clearFilter()

        var index = rosterHandler.indexOf(currentChatId)
        if (index === -1) index = 0
        rosterView.positionViewAtIndex(index, ListView.Center)
    }
}
