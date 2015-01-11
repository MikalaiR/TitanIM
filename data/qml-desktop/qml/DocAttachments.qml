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
import TitanIM.Viewer 1.0

Item {
    id: docAttachments

    implicitWidth: columnDoc.width
    implicitHeight: columnDoc.height

    Component {
        id: docFile

        Row {
            id: content
            width: Math.max(name.contentWidth, description.contentWidth) + img.width + 10
            height: childrenRect.height
            spacing: 7

            Image {
                id: img
                width: 30
                height: 30
                source: "images/attach_doc.png"
            }

            Column {
                width: Math.min(maximumWidth, 240) - img.width
                height: childrenRect.height
                anchors.verticalCenter: img.verticalCenter
                spacing: 3

                Text {
                    id: name
                    anchors.left: parent.left
                    width: parent.width
                    color: "black"
                    font.pointSize: 13
                    font.bold: true
                    font.family: "Helvetica"
                    elide: Text.ElideRight
                    text: model.title
                }

                Text {
                    id: description
                    anchors.left: parent.left
                    width: parent.width
                    color: "grey"
                    font.family: "Helvetica"
                    font.pointSize: 13 - 2
                    font.capitalization: Font.AllUppercase
                    elide: Text.ElideRight
                    text: model.ext + ", " + model.sizeStr
                }
            }
        }
    }

    Component {
        id: docImage

        Rectangle {
            id: content

            readonly property bool isGif: model.ext === "gif"

            width: Math.min(maximumWidth, 130)
            height: 100
            color: "black"

            Image {
                id: img
                anchors.fill: parent
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: model.thumb
            }

            Rectangle {
                id: background
                width: parent.width
                height: title.height + 5
                anchors.bottom: parent.bottom
                opacity: 0.5
                color: "black"
            }

            Text {
                id: title
                width: background.width - (isGif ? sizeLabel.contentWidth + 10 : 0)
                anchors.left: background.left
                anchors.leftMargin: isGif ? 5 : 0
                anchors.verticalCenter: background.verticalCenter
                horizontalAlignment: isGif ? Text.AlignLeft : Text.AlignHCenter
                color: "white"
                font.pointSize: 13 - 2
                elide: Text.ElideMiddle
                text: model.title
            }

            Text {
                id: sizeLabel
                width: background.width - 10
                anchors.right: background.right
                anchors.rightMargin: 5
                anchors.verticalCenter: background.verticalCenter
                horizontalAlignment: Text.AlignRight
                visible: isGif
                color: "white"
                font.pointSize: 13 - 2
                elide: Text.ElideRight
                text: model.sizeStr
            }
        }
    }

    Column {
        id: columnDoc
        spacing: 4

        Repeater {
            model: items

            Loader {
                id: content
                property var model: modelData
                active: true
                sourceComponent: modelData.isImage ? docImage : docFile

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (modelData.ext === "gif") {
                            VideoViewer.setItem(items[index])
                        } else {
                            Qt.openUrlExternally(model.url)
                        }
                    }
                }
            }
        }
    }
}
