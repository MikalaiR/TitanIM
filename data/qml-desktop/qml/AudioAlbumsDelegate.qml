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
    id: audioAlbumsDelegate
    width: audioAlbumsDelegate.ListView.view.width
    height: title.height + 20

    HighlightListView {
        anchors.fill: parent
        visible: albumsView.currentIndex === index
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            albumsView.currentIndex = index
            audio.currentAlbumId = model.albumId
        }
    }

    Row {
        x: 10
        width: parent.width - 5
        anchors.verticalCenter: parent.verticalCenter
        spacing: 7

        Image {
            width: 20
            fillMode: Image.PreserveAspectFit
            source: model.decoration
            visible: false
        }

        Text {
            id: title
            color: "black"
            font.pointSize: main.fontPointSize
            elide: Text.ElideRight
            text: model.display
        }
    }

    SeparatorItem {
        id: separator
        anchors.top: audioAlbumsDelegate.bottom
    }
}
