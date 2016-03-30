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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import TitanIM 2.0
import TitanIM.Viewer 1.0

Item {
    id: photoVideoAttachments

    readonly property int count: items.length

    function __calcHeight(count, size, spacing, width) {
        var col = Math.floor((width + spacing) / (size + spacing))
        var row = Math.ceil(count / col)
        return ((size + spacing) * row) - spacing
    }

    function __calcItemSize(count) {
        switch (count) {
        case 1:
        case 2:
            return 250
        case 3:
        case 4:
        case 5:
        case 6:
            return 120
        default:
            return 80
        }
    }

    implicitWidth: flowPhotos.implicitWidth
    implicitHeight: __calcHeight(items.length, flowPhotos.itemSize, flowPhotos.spacing, maximumWidth)

    Flow {
        id: flowPhotos

        property int itemSize: maximumWidth < __calcItemSize(count) ? maximumWidth : __calcItemSize(count)

        width: maximumWidth
        spacing: 4

        Repeater {
            model: items

            Image {
                id: img
                width: flowPhotos.itemSize
                height: flowPhotos.itemSize
                smooth: true
                fillMode: Image.PreserveAspectCrop
                source: flowPhotos.itemSize > 110 ? modelData.srcBig : modelData.src

                ProgressBar {
                    anchors.left: parent.left
                    anchors.leftMargin: 7
                    anchors.right: parent.right
                    anchors.rightMargin: 7
                    anchors.verticalCenter: parent.verticalCenter
                    maximumValue: 100
                    value: modelData.uploadProgress
                    visible: modelData.uploadProgress !== -1 && modelData.uploadProgress !== 100

                    style: ProgressBarStyle {
                        background: Rectangle {
                            implicitWidth: 150
                            implicitHeight: 7
                            radius: 2
                            color: "lightgray"
                            border.color: "gray"
                            border.width: 1
                        }

                        progress: Rectangle {
                            implicitHeight: 7
                            color: "lightsteelblue"
                            border.color: "steelblue"
                        }
                    }
                }

                Loader {
                    anchors.fill: parent
                    active: modelData.isUploadError

                    sourceComponent: Rectangle {
                        color: "white"
                        opacity: 0.7
                    }
                }

                Loader {
                    property var video: modelData

                    anchors.fill: parent
                    active: modelData.type === Attachment.Video && img.status === Image.Ready
                    source: "VideoLayer.qml"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (modelData.type === Attachment.Photo) {
                            PhotoViewer.setItems(items, index)
                        } else {
                            Qt.openUrlExternally(modelData.link) //todo add videoViewer
                        }
                    }
                }
            }
        }
    }

    onWidthChanged: {
        implicitHeight = __calcHeight(items.length, flowPhotos.itemSize, flowPhotos.spacing, maximumWidth)
    }
}
