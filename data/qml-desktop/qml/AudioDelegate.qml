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
    id: audioDelegate

    property bool playing: (AudioPlayer.audioUrl === model.url) && (AudioPlayer.playing)

    x: 10
    width: audioDelegate.ListView.view.width - 20
    height: audioRow.height + 10

    MouseArea {
        anchors.fill: parent
        onClicked: {
            audio.setCurrentModelIndex(index)
        }
    }

    Row {
        id: audioRow
        height: title.height + artist.height + 5
        width: parent.width - 5
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10

        Image {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            smooth: true
            source: playing ? "images/pause.png" : "images/play.png"
        }

        Column {
            width: parent.width - icon.width - durationText.width - 20
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Text {
                id: title
                width: artist.width
                color: playing ? "#207DCA" : "black"
                font.pointSize: main.fontPointSize - 1
                elide: Text.ElideRight
                text: model.display
            }

            Text {
                id: artist
                width: parent.width - durationText.width - 7
                color: playing ? "#207DCA" : "#505050"
                font.pointSize: main.fontPointSize - 1
                elide: Text.ElideRight
                visible: text.length
                text: model.artist
            }
        }

        Text {
            id: durationText
            anchors.verticalCenter: parent.verticalCenter
            color: playing ? "#207DCA" : "#505050"
            font.pointSize: main.fontPointSize - 2
            text: model.durationStr
        }
    }

    SeparatorItem {
        id: separator
        anchors.top: audioDelegate.bottom
    }
}
