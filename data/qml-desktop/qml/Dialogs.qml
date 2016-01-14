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

    function scrollToTop() {
        dialogsView.positionViewAtBeginning()
    }

    ListView {
        id: dialogsView;
        anchors.fill: parent
        model: dialogsHandler.proxy;
        delegate: DialogsDelegate { }
        spacing: 1
    }

    onCurrentChatIdChanged: {
        if (tabBar.currentIndex === 0) {
            mainWindow.popPage({item: Qt.resolvedUrl("Chat.qml"), immediate: true})
            return;
        }

        var index = dialogsHandler.indexOf(currentChatId)
        if (index === -1) index = 0
        dialogsView.positionViewAtIndex(index, ListView.Center)
    }

    Connections {
        target: dialogsHandler

        onNewMessage: {
            if (id === currentChatId) {
                scrollToTop()
            }
        }
    }
}
