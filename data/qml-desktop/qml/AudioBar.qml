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
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import TitanIM.Multimedia 1.0

Item {
    id: audioBar
    implicitHeight: 45
    implicitWidth: 200

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        anchors.fill: parent
        color: "white"

        Rectangle {
            height: 1
            width: parent.width
            anchors.top: parent.top
            color: "#E8E8E8"
        }

        RowLayout {
            x: 10
            width: parent.width - 20
            height: parent.height
            spacing: 10

            Row {
                id: controlButtons
                spacing: 7

                Image {
                    source: "images/pl_rewind.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            audio.prev()
                        }
                    }
                }

                Image {
                    width: 24
                    source: AudioPlayer.playing ? "images/pl_pause.png" : "images/pl_play.png"
                    fillMode: Image.PreserveAspectFit

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            audio.pause()
                        }
                    }
                }

                Image {
                    source: "images/pl_forward.png"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            audio.next()
                        }
                    }
                }
            }

            Column {
                id: infoLabels
                anchors.verticalCenter: controlButtons.verticalCenter
                anchors.verticalCenterOffset: -2
                spacing: 5
                Layout.fillWidth: true

                Item {
                    width: parent.width
                    height: childrenRect.height

                    Text {
                        id: position
                        anchors.left: parent.left
                        font.pointSize: main.fontPointSize - 2
                        text: audio.durationToText((seekMouseArea.containsMouse ? (seekMouseArea.mouseX / progress.width) * progress.maximumValue : AudioPlayer.position) / 1000)
                        color: "#313030"
                    }

                    Text {
                        id: titleLabel
                        anchors.left: position.right
                        anchors.leftMargin: 7
                        anchors.right: duration.left
                        anchors.rightMargin: 7
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: main.fontPointSize - 1
                        text: audio.currentAudio ? audio.currentAudio.artist + " - " + audio.currentAudio.title : qsTr("No music is playing")
                        elide: Text.ElideRight
                        color: "#313030"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                tabBar.setCurrentItem("audio")
                            }
                        }
                    }

                    Text {
                        id: duration
                        anchors.right: parent.right
                        font.pointSize: main.fontPointSize - 2
                        text: "-" + audio.durationToText((AudioPlayer.duration - AudioPlayer.position) / 1000)
                        color: "#313030"
                    }
                }

                ProgressBar {
                    id: progress
                    width: parent.width
                    height: 4
                    minimumValue: 0
                    maximumValue: AudioPlayer.duration
                    value: AudioPlayer.position

                    style: ProgressBarStyle {
                        background: Rectangle {
                            radius: 10
                            color: "lightsteelblue"
                        }

                        progress: Rectangle {
                            radius: 10
                            color: "#3896CD"
                        }
                    }

                    MouseArea {
                        id: seekMouseArea
                        anchors.fill: parent
                        anchors.topMargin: -7
                        anchors.bottomMargin: -10
                        hoverEnabled: true

                        onClicked: {
                            AudioPlayer.seek((mouse.x / progress.width) * progress.maximumValue)
                        }
                    }
                }
            }

            Slider {
                id: volumeSlider
                anchors.bottom: infoLabels.bottom
                anchors.bottomMargin: -5
                value: AudioPlayer.volume

                style: SliderStyle {
                    groove: Rectangle {
                        implicitWidth: 70
                        implicitHeight: 4
                        color: "#3896CD"
                        radius: 2
                    }

                    handle: Rectangle {
                        anchors.centerIn: parent
                        color: "white"
                        border.color: "#3896CD"
                        border.width: 2
                        implicitWidth: 15
                        implicitHeight: 15
                        radius: 12
                    }
                }

                onValueChanged: {
                    AudioPlayer.volume = value
                }
            }
        }
    }
}
