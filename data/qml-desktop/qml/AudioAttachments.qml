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
import QtQuick.Controls 1.0
import TitanIM.Multimedia 1.0

Item {
    id: audioAttachments

    implicitWidth: columnAudio.width
    implicitHeight: columnAudio.height

    Column {
        id: columnAudio
        spacing: 4

        Repeater {
            model: items

            Row {
                id: audio
                height: title.height + artist.height + 5
                width: Math.min(250, maximumWidth)
                spacing: 10

                Image {
                    id: icon
                    anchors.verticalCenter: parent.verticalCenter
                    smooth: true
                    source: (AudioPlayer.audioUrl === modelData.url) && (AudioPlayer.playing) ?
                                "images/pause.png" : "images/play.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            AudioPlayer.playUrl(modelData.url)
                        }
                    }
                }

                Column {
                    width: parent.width - icon.width - audio.spacing
                    anchors.verticalCenter: parent.verticalCenter

                    Row {
                        Text {
                            id: title
                            width: artist.width
                            color: "black"
                            font.pointSize: main.fontPointSize - 1
                            font.bold: true
                            elide: Text.ElideRight
                            text: modelData.title
                        }

                        Text {
                            id: durationText
                            anchors.bottom: title.bottom
                            color: "#505050"
                            font.pointSize: main.fontPointSize - 2
                            text: modelData.durationStr
                        }
                    }

                    Text {
                        id: artist
                        width: parent.width - durationText.width - 7
                        color: "#505050"
                        font.pointSize: main.fontPointSize - 1
                        elide: Text.ElideRight
                        text: modelData.artist
                    }
                }
            }
        }
    }
}
