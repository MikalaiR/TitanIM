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
import TitanIM.Multimedia 1.0

Item {
    id: audioAlbums
    anchors.fill: parent
    anchors.bottomMargin: 45

    function prepare() {
        if (!albumsView.count) {
            audio.loadAlbums()
            albumsView.currentIndex = 1
            audio.currentAlbumId = -3
        } else {
            if (AudioPlayer.playing) {
                albumsView.currentIndex = 0
                audio.currentAlbumId = -4
            }
        }
    }

    ListView {
        id: albumsView
        anchors.fill: parent
        model: audio.albums
        delegate: AudioAlbumsDelegate { }
        section.property: "section"
        section.labelPositioning: ViewSection.InlineLabels
        section.delegate: Rectangle {
            width: parent.width
            height: 30
            color: "transparent"

            Text {
                x: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -1
                font.pixelSize: 12
                color: "#505050"
                text: section
            }
        }
    }
}
